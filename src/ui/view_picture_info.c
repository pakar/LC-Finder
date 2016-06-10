﻿/* ***************************************************************************
* view_picture_info.c -- picture info view
*
* Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
*
* This file is part of the LC-Finder project, and may only be used, modified,
* and distributed under the terms of the GPLv2.
*
* By continuing to use, modify, or distribute this file you indicate that you
* have read the license and understand and accept it fully.
*
* The LC-Finder project is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
*
* You should have received a copy of the GPLv2 along with this file. It is
* usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
* ****************************************************************************/

/* ****************************************************************************
* view_picture_info.c -- "图片信息" 视图
*
* 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
*
* 这个文件是 LC-Finder 项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和
* 发布。
*
* 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
*
* LC-Finder 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销
* 性或特定用途的隐含担保，详情请参照GPLv2许可协议。
*
* 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在 LICENSE 文件中，如果
* 没有，请查看：<http://www.gnu.org/licenses/>.
* ****************************************************************************/

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "finder.h"
#include "ui.h"
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/cursor.h>
#include <LCUI/graph.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>

#define XML_PATH	"res/ui-view-picture-info.xml"

struct PictureInfoPanel {
	LCUI_Widget panel;
	LCUI_Widget txt_name;
	LCUI_Widget txt_time;
	LCUI_Widget txt_size;
	LCUI_Widget txt_fsize;
	LCUI_Widget txt_dirpath;
	LCUI_Widget view_tags;
	wchar_t *filepath;
	uint64_t size;
	uint_t width;
	uint_t height;
	time_t mtime;
} this_view;

static void OnBtnHideClick( LCUI_Widget w, LCUI_WidgetEvent e, void *arg )
{
	Widget_Hide( this_view.panel );
}

void UI_InitPictureInfoView( void )
{
	LCUI_Widget box, parent, btn;
	box = LCUIBuilder_LoadFile( XML_PATH );
	if( !box ) {
		return;
	}
	Widget_Append( this_view.panel, box );
	Widget_Unwrap( box );
	this_view.filepath = NULL;
	parent = LCUIWidget_GetById( ID_WINDOW_PCITURE_VIEWER );
	btn = LCUIWidget_GetById( ID_BTN_HIDE_PICTURE_INFO );
	this_view.txt_fsize = LCUIWidget_GetById( ID_TXT_PICTURE_FILE_SIZE );
	this_view.txt_size = LCUIWidget_GetById( ID_TXT_PICTURE_SIZE );
	this_view.txt_name = LCUIWidget_GetById( ID_TXT_PICTURE_NAME );
	this_view.txt_dirpath = LCUIWidget_GetById( ID_TXT_PICTURE_PATH );
	this_view.txt_time = LCUIWidget_GetById( ID_TXT_PICTURE_TIME );
	this_view.panel = LCUIWidget_GetById( ID_PANEL_PICTURE_INFO );
	Widget_BindEvent( btn, "click", OnBtnHideClick, NULL, NULL );
	Widget_Append( parent, this_view.panel );
	Widget_Hide( this_view.panel );
}

void UI_SetPictureInfoView( const char *filepath )
{
	size_t len;
	char *apath;
	int width, height;
	wchar_t str[256], *path, *dirpath;
	len = strlen( filepath ) + 1;
	apath = malloc( sizeof( wchar_t ) * len );
	path = malloc( sizeof( wchar_t ) * len );
	dirpath = malloc( sizeof( wchar_t ) * len );
	LCUI_DecodeString( path, filepath, len, ENCODING_UTF8 );
	LCUI_EncodeString( apath, path, len, ENCODING_ANSI );
	TextView_SetTextW( this_view.txt_name, wgetfilename( path ) );
	wgetdirpath( dirpath, len, path );
	TextView_SetTextW( this_view.txt_dirpath, dirpath );
	this_view.mtime = wgetfilemtime( path );
	wgettimestr( str, 256, this_view.mtime );
	TextView_SetTextW( this_view.txt_time, str );
	Graph_GetImageSize( apath, &width, &height );
	swprintf( str, 256, L"%dx%d", width, height );
	TextView_SetTextW( this_view.txt_size, str );
	this_view.width = width;
	this_view.height = height;
	this_view.size = wgetfilesize( path );
	getsizestr( str, 256, this_view.size );
	if( this_view.filepath ) {
		free( this_view.filepath );
	}
	this_view.filepath = path;
}

void UI_ShowPictureInfoView( void )
{
	Widget_Show( this_view.panel );
}

void UI_HidePictureInfoView( void )
{
	Widget_Hide( this_view.panel );
}