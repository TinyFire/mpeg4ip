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
 *              Bill May        wmay@cisco.com
 */
/*
 * gui_private.h - external routines needed by main program.
 */
#ifndef __GUI_PRIVATE_H__
#define __GUI_PRIVATE_H__ 1

#include "mp4live_config.h"
#include "media_flow.h"
#include "preview_flow.h" 

// From gui_main.cpp
extern CLiveConfig* MyConfig;
extern CPreviewAVMediaFlow* AVFlow;
extern SDL_mutex *dialog_mutex;
void NewVideoWindow(void);
void DisplayVideoSettings(void);
void DisplayAudioSettings(void);
void DisplayRecordingSettings(void);
void DisplayTransmitSettings(void);
void DisplayControlSettings(void);
void DisplayStatusSettings(void);
void DisplayAllSettings(void);

void OnAudioProfileFinished(CAudioProfile *p);
void OnVideoProfileFinished(CVideoProfile *p);
void DoStart(void);
void DoStop(void);
void RefreshCurrentStream(void);

// From video_dialog.cpp
void CreateVideoProfileDialog(CVideoProfile *profile);
void CreateVideoDialog(void);

// From picture_dialog.cpp
void CreatePictureDialog(void);

// From audio_dialog.cpp
void CreateAudioProfileDialog(CAudioProfile *profile);
void CreateAudioDialog(void);

// From recording_dialog.cpp
void CreateRecordingDialog(void);

// From transmit_dialog.cpp
void CreateTransmitDialog(void);
void create_IpAddrDialog(CMediaStream *ms, 
			 bool do_audio, 
			 bool do_video, 
			 bool do_text);

// From transcoding_dialog.cpp
void CreateTranscodingDialog(void);

#endif
