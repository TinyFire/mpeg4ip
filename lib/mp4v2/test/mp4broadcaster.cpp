/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is MPEG4IP.
 * 
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2001.  All Rights Reserved.
 * 
 * Contributor(s): 
 *		Dave Mackie		dmackie@cisco.com
 */

#include "mpeg4ip.h"
#include <arpa/inet.h>
#include "mp4.h"

// forward declarations
static bool AssembleSdp(
	MP4FileHandle mp4File, 
	const char* sdpFileName,
	const char* destIpAddress);

static bool InitSockets(
	u_int32_t numSockets, 
	int* pSockets, 
	const char* destIpAddress);

static u_int64_t GetUsecTime();

// globals
char* ProgName;
u_int16_t UdpBasePort = 6970;


// the main show
int main(int argc, char** argv)
{
	// since we're a test program
	// keep command line processing to a minimum
	// and assume some defaults
	ProgName = argv[0];
	char* sdpFileName = "./mp4broadcaster.sdp";
	char* destIpAddress = "224.1.2.3";

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <file>\n", ProgName);
		exit(1);
	}

	char* mp4FileName = argv[1];
	u_int32_t verbosity = MP4_DETAILS_ERROR;

	// open the mp4 file
	MP4FileHandle mp4File = MP4Read(mp4FileName, verbosity);

	if (mp4File == MP4_INVALID_FILE_HANDLE) {
		// library will print an error message
		exit(1);
	}

	// check for hint tracks
	u_int32_t numHintTracks = 
		MP4GetNumberOfTracks(mp4File, MP4_HINT_TRACK_TYPE);

	if (numHintTracks == 0) {
		fprintf(stderr, "%s: File %s does not contain any hint tracks", 
			ProgName, mp4FileName);
		exit(2);
	}

	// assemble and write out sdp file
	AssembleSdp(mp4File, sdpFileName, destIpAddress);

	// create a UDP socket for each track that will be streamed
	int* udpSockets = new int[numHintTracks]; 

	if (!InitSockets(numHintTracks, udpSockets, destIpAddress)) {
		fprintf(stderr, "%s: Couldn't create UDP sockets\n",
			ProgName);
		exit(3);
	}

	// initialize per track variables that we will use in main loop
	MP4TrackId* hintTrackIds = new MP4TrackId[numHintTracks]; 
	MP4SampleId* nextHintIds = new MP4SampleId[numHintTracks]; 
	MP4SampleId* maxHintIds = new MP4SampleId[numHintTracks]; 
	u_int64_t* nextHintTimes = new MP4Timestamp[numHintTracks];

	u_int32_t i;

	for (i = 0; i < numHintTracks; i++) {
		hintTrackIds[i] = MP4FindTrackId(mp4File, i, MP4_HINT_TRACK_TYPE);
		nextHintIds[i] = 1;
		maxHintIds[i] = MP4GetTrackNumberOfSamples(mp4File, hintTrackIds[i]);
		nextHintTimes[i] = (u_int64_t)-1;
	}

	// remember the starting time
	u_int64_t start = GetUsecTime();
	u_int32_t ssrc = random();

	// main loop to stream data
	// note: a real application would also need to periodically
	// send RTCP SR (sender reports) to the udp port + 1
	while (true) {
		u_int32_t nextTrackIndex = (u_int32_t)-1;
		u_int64_t nextTime = (u_int64_t)-1;

		// find the next hint to send
		for (i = 0; i < numHintTracks; i++) {
			if (nextHintIds[i] > maxHintIds[i]) {
				// have finished this track
				continue;
			}

			// need to get the time of the next hint
			if (nextHintTimes[i] == (u_int64_t)-1) {
				MP4Timestamp hintTime =
					MP4GetSampleTime(mp4File, hintTrackIds[i], nextHintIds[i]);

				nextHintTimes[i] = 
					MP4ConvertFromTrackTimestamp(mp4File, hintTrackIds[i],
						hintTime, MP4_USECS_TIME_SCALE);
			}

			// check if this track's next hint is the earliest yet
			if (nextHintTimes[i] > nextTime) {
				continue;
			}

			// make this our current choice for the next hint
			nextTime = nextHintTimes[i];
			nextTrackIndex = i;
		}

		// check exit condition, i.e all hints for all tracks have been used
		if (nextTrackIndex == (u_int32_t)-1) {
			break;
		}

		// wait until the correct time to send next hint
		// we assume we're not going to fall behind for testing purposes
		// in a real application some skipping of samples might be needed

		u_int64_t now = GetUsecTime();
		int64_t waitTime = (start + nextTime) - now;
		if (waitTime > 0) {
			usleep(waitTime);
		}

		// send all the packets for this hint
		// since this is just a test program 
		// we don't attempt to smooth out the transmission of the packets

		u_int16_t numPackets;

		MP4ReadRtpHint(
			mp4File, 
			hintTrackIds[nextTrackIndex], 
			nextHintIds[nextTrackIndex],
			&numPackets);

		u_int16_t packetIndex;

		for (packetIndex = 0; packetIndex < numPackets; packetIndex++) {
			u_int8_t* pPacket = NULL;
			u_int32_t packetSize;

			// get the packet from the library
			MP4ReadRtpPacket(
				mp4File, 
				hintTrackIds[nextTrackIndex], 
				packetIndex,
				&pPacket,
				&packetSize,
				ssrc);

			if (pPacket == NULL) {
				// error, but forge on
				continue;
			}

			// send it out via UDP
			send(udpSockets[nextTrackIndex], pPacket, packetSize, 0);

			// free packet buffer
			free(pPacket);
		}
	}

	// main loop finished

	// close the UDP sockets
	for (i = 0; i < numHintTracks; i++) {
		close(udpSockets[i]);
	}

	// close mp4 file
	MP4Close(mp4File);

	// free up memory
	delete [] hintTrackIds;
	delete [] nextHintIds;
	delete [] maxHintIds;
	delete [] nextHintTimes;

	exit(0);
}

static bool AssembleSdp(
	MP4FileHandle mp4File, 
	const char* sdpFileName,
	const char* destIpAddress)
{
	// open the destination sdp file
	FILE* sdpFile = fopen(sdpFileName, "w");

	if (sdpFile == NULL) {
		fprintf(stderr, 
			"%s: couldn't open sdp file %s\n",
			ProgName, sdpFileName);
		return false;
	}

	// add required header fields
	fprintf(sdpFile,
		"v=0\015\012"
		"o=- 1 1 IN IP4 127.0.0.1\015\012"
		"s=mp4broadcaster\015\012"
		"e=NONE\015\012"
		"c=IN IP4 %s/1"
		"b=RR:0\015\012",
		destIpAddress);

	// add session level info from mp4 file
	fprintf(sdpFile, "%s", 
		MP4GetSessionSdp(mp4File));

	// add per track info
	u_int32_t numHintTracks = 
		MP4GetNumberOfTracks(mp4File, MP4_HINT_TRACK_TYPE);

	for (u_int32_t i = 0; i < numHintTracks; i++) {
		MP4TrackId hintTrackId = 
			MP4FindTrackId(mp4File, i, MP4_HINT_TRACK_TYPE);

		if (hintTrackId == MP4_INVALID_TRACK_ID) {
			continue;
		}

		// get track sdp info from mp4 file
		const char* mediaSdp =
			MP4GetHintTrackSdp(mp4File, hintTrackId);

		// since we're going to broadcast instead of use RTSP for on-demand
		// we need to fill in the UDP port numbers for the media
		const char* portZero = strchr(mediaSdp, '0');
		if (portZero == NULL) {
			continue;	// mal-formed sdp
		}
		fprintf(sdpFile, "%*s%u%s",
			portZero - mediaSdp,
			mediaSdp,
			UdpBasePort + (i * 2),
			portZero + 1);
	}

	fclose(sdpFile);

	return true;
}

static bool InitSockets(
	u_int32_t numSockets, 
	int* pSockets, 
	const char* destIpAddress)
{
	u_int32_t i;


	for (i = 0; i < numSockets; i++) {
		pSockets[i] = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (pSockets[i] < 0) {
			return false;
		}

		struct sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = 0;
		sin.sin_addr.s_addr = INADDR_ANY;

		if (bind(pSockets[i], (struct sockaddr*)&sin, sizeof(sin))) {
			return false;
		}

		sin.sin_port = UdpBasePort + (i * 2);
		inet_aton(destIpAddress, &sin.sin_addr);

		if (connect(pSockets[i], (struct sockaddr*)&sin, sizeof(sin)) < 0) {
			return false;
		}

		// Note real application would set multicast ttl here
	}

	return true;
}

// get absolute time expressed in microseconds
static u_int64_t GetUsecTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}
