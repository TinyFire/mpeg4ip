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

/* 
 * MP4 library API functions
 * 
 * These are wrapper functions that provide C linkage conventions
 * to the library, and catch any internal errors, ensuring that
 * a proper return value is given.
 */

#include "mp4common.h"

#define PRINT_ERROR(e) \
	VERBOSE_ERROR(((MP4File*)hFile)->GetVerbosity(), e->Print());

/* file operations */

extern "C" MP4FileHandle MP4Read(const char* fileName, u_int32_t verbosity)
{
	MP4File* pFile = NULL;
	try {
		pFile = new MP4File(verbosity);
		pFile->Read(fileName);
		return (MP4FileHandle)pFile;
	}
	catch (MP4Error* e) {
		VERBOSE_ERROR(verbosity, e->Print());
		delete e;
		delete pFile;
		return MP4_INVALID_FILE_HANDLE;
	}
}

extern "C" MP4FileHandle MP4Create(const char* fileName, 
	u_int32_t verbosity, bool use64bits, bool useExtensibleFormat)
{
	MP4File* pFile = NULL;
	try {
		pFile = new MP4File(verbosity);
		// LATER useExtensibleFormat, moov first, then mvex's
		pFile->Create(fileName, use64bits);
		return (MP4FileHandle)pFile;
	}
	catch (MP4Error* e) {
		VERBOSE_ERROR(verbosity, e->Print());
		delete e;
		delete pFile;
		return MP4_INVALID_FILE_HANDLE;
	}
}

extern "C" MP4FileHandle MP4Modify(const char* fileName, 
	u_int32_t verbosity, bool useExtensibleFormat)
{
	MP4File* pFile = NULL;
	try {
		pFile = new MP4File(verbosity);
		// LATER useExtensibleFormat, moov first, then mvex's
		pFile->Modify(fileName);
		return (MP4FileHandle)pFile;
	}
	catch (MP4Error* e) {
		VERBOSE_ERROR(verbosity, e->Print());
		delete e;
		delete pFile;
		return MP4_INVALID_FILE_HANDLE;
	}
}

extern "C" bool MP4Optimize(const char* existingFileName, 
	const char* newFileName, 
	u_int32_t verbosity)
{
	try {
		MP4File* pFile = new MP4File(verbosity);
		pFile->Optimize(existingFileName, newFileName);
		delete pFile;
		return true;
	}
	catch (MP4Error* e) {
		VERBOSE_ERROR(verbosity, e->Print());
		delete e;
	}
	return false;
}

extern "C" bool MP4Close(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->Close();
			delete (MP4File*)hFile;
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4Dump(MP4FileHandle hFile, FILE* pDumpFile, bool dumpImplicits)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->Dump(pDumpFile, dumpImplicits);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}


/* specific file properties */

extern "C" u_int32_t MP4GetVerbosity(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetVerbosity();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4SetVerbosity(MP4FileHandle hFile, u_int32_t verbosity)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetVerbosity(verbosity);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" MP4Duration MP4GetDuration(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetDuration();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_DURATION;
}

extern "C" u_int32_t MP4GetTimeScale(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTimeScale();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4SetTimeScale(MP4FileHandle hFile, u_int32_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetTimeScale(value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int8_t MP4GetODProfileLevel(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetODProfileLevel();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4SetODProfileLevel(MP4FileHandle hFile, u_int8_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetODProfileLevel(value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int8_t MP4GetSceneProfileLevel(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetSceneProfileLevel();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4SetSceneProfileLevel(MP4FileHandle hFile, u_int8_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetSceneProfileLevel(value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int8_t MP4GetVideoProfileLevel(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetVideoProfileLevel();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4SetVideoProfileLevel(MP4FileHandle hFile, u_int8_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetVideoProfileLevel(value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int8_t MP4GetAudioProfileLevel(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetAudioProfileLevel();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4SetAudioProfileLevel(MP4FileHandle hFile, u_int8_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetAudioProfileLevel(value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int8_t MP4GetGraphicsProfileLevel(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetGraphicsProfileLevel();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4SetGraphicsProfileLevel(MP4FileHandle hFile, u_int8_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetGraphicsProfileLevel(value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

/* generic file properties */

extern "C" u_int64_t MP4GetIntegerProperty(
	MP4FileHandle hFile, const char* propName)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetIntegerProperty(propName);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return (u_int64_t)-1;
}

extern "C" float MP4GetFloatProperty(
	MP4FileHandle hFile, const char* propName)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetFloatProperty(propName);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return NAN;
}

extern "C" const char* MP4GetStringProperty(
	MP4FileHandle hFile, const char* propName)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetStringProperty(propName);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return NULL;
}

extern "C" void MP4GetBytesProperty(
	MP4FileHandle hFile, const char* propName, 
	u_int8_t** ppValue, u_int32_t* pValueSize)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->GetBytesProperty(propName, ppValue, pValueSize);
			return;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	*ppValue = NULL;
	*pValueSize = 0;
	return;
}

extern "C" bool MP4SetIntegerProperty(
	MP4FileHandle hFile, const char* propName, int64_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetIntegerProperty(propName, value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4SetFloatProperty(
	MP4FileHandle hFile, const char* propName, float value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetFloatProperty(propName, value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4SetStringProperty(
	MP4FileHandle hFile, const char* propName, const char* value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetStringProperty(propName, value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4SetBytesProperty(
	MP4FileHandle hFile, const char* propName, 
	const u_int8_t* pValue, u_int32_t valueSize)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetBytesProperty(propName, pValue, valueSize);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

/* track operations */

extern "C" MP4TrackId MP4AddTrack(
	MP4FileHandle hFile, const char* type)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->AddSystemsTrack(type);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" MP4TrackId MP4AddSystemsTrack(
	MP4FileHandle hFile, const char* type)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->AddSystemsTrack(type);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" MP4TrackId MP4AddODTrack(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->AddODTrack();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" MP4TrackId MP4AddSceneTrack(MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->AddSceneTrack();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" MP4TrackId MP4AddAudioTrack(MP4FileHandle hFile, 
	u_int32_t timeScale, u_int32_t sampleDuration, u_int8_t audioType)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->
				AddAudioTrack(timeScale, sampleDuration, audioType);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" MP4TrackId MP4AddVideoTrack(MP4FileHandle hFile, 
	u_int32_t timeScale, u_int32_t sampleDuration,
	u_int16_t width, u_int16_t height, u_int8_t videoType)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->AddVideoTrack(
				timeScale, sampleDuration, width, height, videoType);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" MP4TrackId MP4AddHintTrack(
	MP4FileHandle hFile, MP4TrackId refTrackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->AddHintTrack(refTrackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" bool MP4DeleteTrack(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->DeleteTrack(trackId);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int32_t MP4GetNumberOfTracks(
	MP4FileHandle hFile, 
	const char* type,
	u_int8_t subType)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetNumberOfTracks(type, subType);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" MP4TrackId MP4FindTrackId(
	MP4FileHandle hFile, 
	u_int16_t index, 
	const char* type,
	u_int8_t subType)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->FindTrackId(index, type, subType);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" u_int16_t MP4FindTrackIndex(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->FindTrackIndex(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return (u_int16_t)-1;
}

/* specific track properties */

extern "C" const char* MP4GetTrackType(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackType(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return NULL;
}

extern "C" MP4Duration MP4GetTrackDuration(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackDuration(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_DURATION;
}

extern "C" u_int32_t MP4GetTrackTimeScale(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackTimeScale(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4SetTrackTimeScale(
	MP4FileHandle hFile, MP4TrackId trackId, u_int32_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetTrackTimeScale(trackId, value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int8_t MP4GetTrackAudioType(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackAudioType(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_AUDIO_TYPE;
}

extern "C" u_int8_t MP4GetTrackVideoType(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackVideoType(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_VIDEO_TYPE;
}

extern "C" MP4Duration MP4GetTrackFixedSampleDuration(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackFixedSampleDuration(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_DURATION;
}

extern "C" void MP4GetTrackESConfiguration(
	MP4FileHandle hFile, MP4TrackId trackId, 
	u_int8_t** ppConfig, u_int32_t* pConfigSize)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->GetTrackESConfiguration(
				trackId, ppConfig, pConfigSize);
			return;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	*ppConfig = NULL;
	*pConfigSize = 0;
	return;
}

extern "C" bool MP4SetTrackESConfiguration(
	MP4FileHandle hFile, MP4TrackId trackId, 
	const u_int8_t* pConfig, u_int32_t configSize)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetTrackESConfiguration(
				trackId, pConfig, configSize);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" MP4SampleId MP4GetTrackNumberOfSamples(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackNumberOfSamples(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" u_int16_t MP4GetTrackVideoWidth(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackIntegerProperty(trackId,
				"mdia.minf.stbl.stsd.mp4v.width");
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" u_int16_t MP4GetTrackVideoHeight(
	MP4FileHandle hFile, MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackIntegerProperty(trackId,
				"mdia.minf.stbl.stsd.mp4v.height");
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

/* generic track properties */

extern "C" u_int64_t MP4GetTrackIntegerProperty(
	MP4FileHandle hFile, MP4TrackId trackId, 
	const char* propName)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackIntegerProperty(trackId, 
				propName);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return (u_int64_t)-1;
}

extern "C" float MP4GetTrackFloatProperty(
	MP4FileHandle hFile, MP4TrackId trackId, 
	const char* propName)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackFloatProperty(trackId, propName);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return NAN;
}

extern "C" const char* MP4GetTrackStringProperty(
	MP4FileHandle hFile, MP4TrackId trackId, 
	const char* propName)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackStringProperty(trackId, propName);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return NULL;
}

extern "C" void MP4GetTrackBytesProperty(
	MP4FileHandle hFile, MP4TrackId trackId, const char* propName, 
	u_int8_t** ppValue, u_int32_t* pValueSize)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->GetTrackBytesProperty(
				trackId, propName, ppValue, pValueSize);
			return;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	*ppValue = NULL;
	*pValueSize = 0;
	return;
}

extern "C" bool MP4SetTrackIntegerProperty(
	MP4FileHandle hFile, MP4TrackId trackId, 
	const char* propName, int64_t value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetTrackIntegerProperty(trackId, 
				propName, value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4SetTrackFloatProperty(
	MP4FileHandle hFile, MP4TrackId trackId, 
	const char* propName, float value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetTrackFloatProperty(trackId, propName, value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4SetTrackStringProperty(
	MP4FileHandle hFile, MP4TrackId trackId, 
	const char* propName, const char* value)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetTrackStringProperty(trackId, propName, value);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4SetTrackBytesProperty(
	MP4FileHandle hFile, MP4TrackId trackId, 
	const char* propName, const u_int8_t* pValue, u_int32_t valueSize)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetTrackBytesProperty(
				trackId, propName, pValue, valueSize);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

/* sample operations */

extern "C" bool MP4ReadSample(
	/* input parameters */
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4SampleId sampleId,
	/* output parameters */
	u_int8_t** ppBytes, 
	u_int32_t* pNumBytes, 
	MP4Timestamp* pStartTime, 
	MP4Duration* pDuration,
	MP4Duration* pRenderingOffset, 
	bool* pIsSyncSample)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->ReadSample(trackId, sampleId, ppBytes, pNumBytes, 
				pStartTime, pDuration, pRenderingOffset, pIsSyncSample);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	*pNumBytes = 0;
	return false;
}

extern "C" bool MP4WriteSample(
	MP4FileHandle hFile,
	MP4TrackId trackId,
	u_int8_t* pBytes, 
	u_int32_t numBytes,
	MP4Duration duration,
	MP4Duration renderingOffset, 
	bool isSyncSample)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->WriteSample(trackId, pBytes, numBytes, 
				duration, renderingOffset, isSyncSample);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int32_t MP4GetSampleSize(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4SampleId sampleId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetSampleSize(
				trackId, sampleId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" u_int32_t MP4GetTrackMaxSampleSize(
	MP4FileHandle hFile,
	MP4TrackId trackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetTrackMaxSampleSize(trackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" MP4SampleId MP4GetSampleIdFromTime(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4Timestamp when, 
	bool wantSyncSample)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetSampleIdFromTime(
				trackId, when, wantSyncSample);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_SAMPLE_ID;
}

extern "C" MP4Timestamp MP4GetSampleTime(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4SampleId sampleId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetSampleTime(
				trackId, sampleId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TIMESTAMP;
}

extern "C" MP4Duration MP4GetSampleDuration(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4SampleId sampleId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetSampleDuration(
				trackId, sampleId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_DURATION;
}

extern "C" MP4Duration MP4GetSampleRenderingOffset(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4SampleId sampleId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetSampleRenderingOffset(
				trackId, sampleId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_DURATION;
}

extern "C" bool MP4SetSampleRenderingOffset(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4SampleId sampleId,
	MP4Duration renderingOffset)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetSampleRenderingOffset(
				trackId, sampleId, renderingOffset);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" int8_t MP4GetSampleSync(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4SampleId sampleId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetSampleSync(
				trackId, sampleId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return -1;
}


extern "C" u_int64_t MP4ConvertFromMovieDuration(
	MP4FileHandle hFile,
	MP4Duration duration,
	u_int32_t timeScale)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->ConvertFromMovieDuration(
				duration, timeScale);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return (u_int64_t)MP4_INVALID_DURATION;
}

extern "C" u_int64_t MP4ConvertFromTrackTimestamp(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4Timestamp timeStamp,
	u_int32_t timeScale)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->ConvertFromTrackTimestamp(
				trackId, timeStamp, timeScale);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return (u_int64_t)MP4_INVALID_TIMESTAMP;
}

extern "C" MP4Timestamp MP4ConvertToTrackTimestamp(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	u_int64_t timeStamp,
	u_int32_t timeScale)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->ConvertToTrackTimestamp(
				trackId, timeStamp, timeScale);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TIMESTAMP;
}

extern "C" u_int64_t MP4ConvertFromTrackDuration(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	MP4Duration duration,
	u_int32_t timeScale)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->ConvertFromTrackDuration(
				trackId, duration, timeScale);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return (u_int64_t)MP4_INVALID_DURATION;
}

extern "C" MP4Duration MP4ConvertToTrackDuration(
	MP4FileHandle hFile,
	MP4TrackId trackId, 
	u_int64_t duration,
	u_int32_t timeScale)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->ConvertToTrackDuration(
				trackId, duration, timeScale);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_DURATION;
}

extern "C" bool MP4GetHintTrackRtpPayload(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	char** ppPayloadName,
	u_int8_t* pPayloadNumber,
	u_int16_t* pMaxPayloadSize)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->GetHintTrackRtpPayload(
				hintTrackId, ppPayloadName, pPayloadNumber, pMaxPayloadSize);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4SetHintTrackRtpPayload(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	const char* pPayloadName,
	u_int8_t* pPayloadNumber,
	u_int16_t maxPayloadSize)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetHintTrackRtpPayload(
				hintTrackId, pPayloadName, pPayloadNumber, maxPayloadSize);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" const char* MP4GetSessionSdp(
	MP4FileHandle hFile)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetSessionSdp();
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return NULL;
}

extern "C" bool MP4SetSessionSdp(
	MP4FileHandle hFile,
	const char* sdpString)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetSessionSdp(sdpString);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4AppendSessionSdp(
	MP4FileHandle hFile,
	const char* sdpString)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->AppendSessionSdp(sdpString);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" const char* MP4GetHintTrackSdp(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetHintTrackSdp(hintTrackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return NULL;
}

extern "C" bool MP4SetHintTrackSdp(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	const char* sdpString)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetHintTrackSdp(hintTrackId, sdpString);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4AppendHintTrackSdp(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	const char* sdpString)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->AppendHintTrackSdp(hintTrackId, sdpString);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" MP4TrackId MP4GetHintTrackReferenceTrackId(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->
				GetHintTrackReferenceTrackId(hintTrackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TRACK_ID;
}

extern "C" bool MP4ReadRtpHint(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	MP4SampleId hintSampleId,
	u_int16_t* pNumPackets)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->ReadRtpHint(
				hintTrackId, hintSampleId, pNumPackets);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" u_int16_t MP4GetRtpHintNumberOfPackets(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetRtpHintNumberOfPackets(hintTrackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" int8_t MP4GetRtpPacketBFrame(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	u_int16_t packetIndex)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->
				GetRtpPacketBFrame(hintTrackId, packetIndex);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return -1;
}

extern "C" int32_t MP4GetRtpPacketTransmitOffset(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	u_int16_t packetIndex)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->
				GetRtpPacketTransmitOffset(hintTrackId, packetIndex);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return 0;
}

extern "C" bool MP4ReadRtpPacket(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	u_int16_t packetIndex,
	u_int8_t** ppBytes, 
	u_int32_t* pNumBytes,
	u_int32_t ssrc,
	bool includeHeader,
	bool includePayload)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->ReadRtpPacket(
				hintTrackId, packetIndex, 
				ppBytes, pNumBytes, 
				ssrc, includeHeader, includePayload);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" MP4Timestamp MP4GetRtpTimestampStart(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			return ((MP4File*)hFile)->GetRtpTimestampStart(hintTrackId);
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return MP4_INVALID_TIMESTAMP;
}

extern "C" bool MP4SetRtpTimestampStart(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	MP4Timestamp rtpStart)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->SetRtpTimestampStart(
				hintTrackId, rtpStart);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4AddRtpHint(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId)
{
	return MP4AddRtpVideoHint(hFile, hintTrackId, false, 0);
}

extern "C" bool MP4AddRtpVideoHint(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	bool isBframe, 
	u_int32_t timestampOffset)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->AddRtpHint(hintTrackId, 
				isBframe, timestampOffset);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4AddRtpPacket(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	bool setMbit,
	int32_t transmitOffset)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->AddRtpPacket(
				hintTrackId, setMbit, transmitOffset);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4AddRtpImmediateData(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	const u_int8_t* pBytes,
	u_int32_t numBytes)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->AddRtpImmediateData(hintTrackId, 
				pBytes, numBytes);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4AddRtpSampleData(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	MP4SampleId sampleId,
	u_int32_t dataOffset,
	u_int32_t dataLength)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->AddRtpSampleData(
				hintTrackId, sampleId, dataOffset, dataLength);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4AddRtpESConfigurationPacket(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->AddRtpESConfigurationPacket(hintTrackId);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

extern "C" bool MP4WriteRtpHint(
	MP4FileHandle hFile,
	MP4TrackId hintTrackId,
	MP4Duration duration,
	bool isSyncSample)
{
	if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
		try {
			((MP4File*)hFile)->WriteRtpHint(
				hintTrackId, duration, isSyncSample);
			return true;
		}
		catch (MP4Error* e) {
			PRINT_ERROR(e);
			delete e;
		}
	}
	return false;
}

/* ISMA specific operations */

extern "C" bool MP4MakeIsmaCompliant(
	const char* fileName, 
	u_int32_t verbosity,
	bool addIsmaComplianceSdp)
{
	try {
		MP4File* pFile = new MP4File(verbosity);
		pFile->Modify(fileName);
		pFile->MakeIsmaCompliant(addIsmaComplianceSdp);
		pFile->Close();
		delete pFile;
		return true;
	}
	catch (MP4Error* e) {
		VERBOSE_ERROR(verbosity, e->Print());
		delete e;
	}
	return false;
}

/* Utlities */

extern "C" char* MP4BinaryToBase16(
	const u_int8_t* pData, 
	u_int32_t dataSize)
{
	if (pData || dataSize == 0) {
		try {
			return MP4ToBase16(pData, dataSize);
		}
		catch (MP4Error* e) {
			delete e;
		}
	}
	return NULL;
}

extern "C" char* MP4BinaryToBase64(
	const u_int8_t* pData, 
	u_int32_t dataSize)
{
	if (pData || dataSize == 0) {
		try {
			return MP4ToBase64(pData, dataSize);
		}
		catch (MP4Error* e) {
			delete e;
		}
	}
	return NULL;
}

