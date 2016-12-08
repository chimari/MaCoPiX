/*
	TAR32API.H
		TAR32.DLL API header.
		by Yoshioka Tsuneo(QWF00133@nifty.ne.jp)
*/
/*	
	このファイルの利用条件：
		このソースファイルの利用制限は一切ありません。
		ソースの一部、全部を商用、非商用など目的に
		かかわりなく他のプログラムで自由に使用できます。
		パブリック・ドメイン・ソフトウェアと同様に扱えます。
	
	プログラマ向けの要望(制限ではありません)：
		ソース中に改善すべき点があればお知らせください。
		ソースコード中にバグを見つけた場合は報告してください。
		直した部分などありましたら教えてください。
		断片的な情報でも結構です。
		このファイルを利用した場合はなるべく教えてください。
*/
/*
	LICENSE of this file:
		There is no restriction for using this file.
		You can use this file in your software for any purpose.
		In other words, you can use this file as Public Domain Software.

	RECOMMENDATION for Programmer(not restriction):
		If you find points to improve code, please report me.
		If you find bug in source code, please report me.
		If you fixed bug, please teach me.
		I want any trivial information.
		If you use this file, please report me.
*/

#ifndef TAR32API_H
#define TAR32API_H

#ifndef FNAME_MAX32
#define FNAME_MAX32		512
#endif
#if defined(__BORLANDC__)
#pragma option -a-
#else
#pragma pack(1)
#endif

#ifndef _TIME_T_DEFINED
typedef long time_t;
#define _TIME_T_DEFINED
#endif

#ifndef ARC_DECSTRACT
#define ARC_DECSTRACT
/* #define void *HGLOBAL */

#include <wtypes.h>

typedef	HGLOBAL	HARC;

typedef struct {
	DWORD 			dwOriginalSize;
	DWORD 			dwCompressedSize;
	DWORD			dwCRC;
	UINT			uFlag;
	UINT			uOSType;
	WORD			wRatio;
	WORD			wDate;
	WORD 			wTime;
	char			szFileName[FNAME_MAX32 + 1];
	char			dummy1[3];
	char			szAttribute[8];
	char			szMode[8];
}	INDIVIDUALINFO;

typedef INDIVIDUALINFO *LPINDIVIDUALINFO;

/* only declare struct( NO IMPLEMENTED )*/
typedef struct {
	DWORD 			dwFileSize;
	DWORD			dwWriteSize;
	char			szSourceFileName[FNAME_MAX32 + 1];
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
	char			dummy[3];
}	EXTRACTINGINFO;
typedef struct {
	EXTRACTINGINFO exinfo;
	DWORD dwCompressedSize;
	DWORD dwCRC;
	UINT  uOSType;
	WORD  wRatio;
	WORD  wDate;
	WORD  wTime;
	char  szAttribute[8];
	char  szMode[8];
} EXTRACTINGINFOEX;
typedef BOOL CALLBACK ARCHIVERPROC(HWND _hwnd,UINT _uMsg,UINT _nState, EXTRACTINGINFOEX * _lpEis);

#endif

#if !defined(__BORLANDC__)
#pragma pack()
#else
#pragma option -a.
#endif

#if !defined(__BORLANDC__)
#define	_export
#endif

#ifdef __cplusplus
extern "C" {
#endif
WORD WINAPI _export TarGetVersion(VOID);

BOOL WINAPI _export TarGetRunning(VOID);

BOOL WINAPI _export TarGetBackGroundMode(VOID);/* NO IMPLEMENT:return FALSE */
BOOL WINAPI _export TarSetBackGroundMode(const BOOL _BackGroundMode);/* NO IMPLEMENT:return FALSE */
BOOL WINAPI _export TarGetCursorMode(VOID);/* NO IMPLEMENT:return FALSE */
BOOL WINAPI _export TarSetCursorMode(const BOOL _CursorMode);/* NO IMPLEMENT:return FALSE */
WORD WINAPI _export TarGetCursorInterval(VOID);/* NO IMPLEMENT:return 80 */
BOOL WINAPI _export TarSetCursorInterval(const WORD _Interval);/* NO IMPLEMENT:return FALSE */

int WINAPI _export Tar(const HWND _hwnd, LPCSTR _szCmdLine,LPSTR _szOutput, const DWORD _dwSize);
/* NO IMPLEMENT:return -1 */
int WINAPI _export TarExtractMem(const HWND _hwndParent,LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize,time_t *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize);
/* NO IMPLEMENT:return -1 */
int WINAPI _export TarCompressMem(const HWND _hwndParent,LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize,const time_t *_lpTime, const LPWORD _lpwAttr,LPDWORD _lpdwWriteSize);
BOOL WINAPI _export TarCheckArchive(LPCSTR _szFileName, const int _iMode);
/* Simple Dialog Only :return TRUE */
BOOL WINAPI _export TarConfigDialog(const HWND _hwnd, LPSTR _lpszComBuffer,const int _iMode);
int WINAPI _export TarGetFileCount(LPCSTR _szArcFile);
HARC WINAPI _export TarOpenArchive(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode);
int WINAPI _export TarCloseArchive(HARC _harc);
int WINAPI _export TarFindFirst(HARC _harc, LPCSTR _szWildName,INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export TarFindNext(HARC _harc, INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export TarGetArcFileName(HARC _harc, LPSTR _lpBuffer,const int _nSize);

DWORD WINAPI _export TarGetArcFileSize(HARC _harc);
DWORD WINAPI _export TarGetArcOriginalSize(HARC _harc);
/* NO IMPLEMENT:return 0: */
DWORD WINAPI _export TarGetArcCompressedSize(HARC _harc);
/* NO IMPLEMENT:return 0: */
WORD WINAPI _export TarGetArcRatio(HARC _harc);
WORD WINAPI _export TarGetArcDate(HARC _harc);
WORD WINAPI _export TarGetArcTime(HARC _harc);
/* NO IMPLEMENT:return -1: */
UINT WINAPI _export TarGetArcOSType(HARC _harc);
int WINAPI _export TarGetFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
int WINAPI _export TarGetMethod(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
BOOL WINAPI _export TarGetOriginalSizeEx(HARC _harc, __int64 *_lpllSize);
DWORD WINAPI _export TarGetOriginalSize(HARC _harc);
BOOL WINAPI _export TarGetCompressedSizeEx(HARC _harc, __int64 *_lpllSize);
DWORD WINAPI _export TarGetCompressedSize(HARC _harc);
WORD WINAPI _export TarGetRatio(HARC _harc);
WORD WINAPI _export TarGetDate(HARC _harc);
WORD WINAPI _export TarGetTime(HARC _harc);
DWORD WINAPI _export TarGetWriteTime(HARC _harc);
DWORD WINAPI _export TarGetAccessTime(HARC _harc);
DWORD WINAPI _export TarGetCreateTime(HARC _harc);
DWORD WINAPI _export TarGetCRC(HARC _harc);
int WINAPI _export TarGetAttribute(HARC _harc);
UINT WINAPI _export TarGetOSType(HARC _harc);

BOOL WINAPI _export TarQueryFunctionList(const int _iFunction);

BOOL WINAPI _export TarSetOwnerWindow(const HWND _hwnd);
BOOL WINAPI _export TarClearOwnerWindow(void);
BOOL WINAPI _export TarSetOwnerWindowEx(HWND _hwnd,ARCHIVERPROC *_lpArcProc);
BOOL WINAPI _export TarKillOwnerWindowEx(HWND _hwnd);

int WINAPI _export TarGetArchiveType(LPCSTR _szFileName);

#ifdef __cplusplus
}
#endif

#define	WM_ARCEXTRACT	"wm_arcextract"

#define	ARCEXTRACT_BEGIN		0	/* 該当ファイルの処理の開始 */
#define	ARCEXTRACT_INPROCESS	1	/* 該当ファイルの展開中 */
#define	ARCEXTRACT_END			2	/* 処理終了、関連メモリを開放 */
#define ARCEXTRACT_OPEN			3	/* 該当書庫の処理の開始 */
#define ARCEXTRACT_COPY			4	/* ワークファイルの書き戻し */

#define	UNPACK_CONFIG_MODE		1
#define PACK_CONFIG_MODE		2

#define	CHECKARCHIVE_RAPID		0
#define	CHECKARCHIVE_BASIC		1
#define	CHECKARCHIVE_FULLCRC	2

#if !defined(EXTRACT_FOUND_FILE)
/* MODE (for ???OpenArchive) */
#define M_INIT_FILE_USE			0x00000001L
#define M_REGARDLESS_INIT_FILE	0x00000002L
#define M_CHECK_ALL_PATH		0x00000100L
#define M_CHECK_FILENAME_ONLY	0x00000200L
#define M_USE_DRIVE_LETTER		0x00001000L
#define M_NOT_USE_DRIVE_LETTER	0x00002000L
#define M_ERROR_MESSAGE_ON		0x00400000L
#define M_ERROR_MESSAGE_OFF		0x00800000L
#define M_ERROR_MESSAGE_ON		0x00400000L	/* エラーメッセージを表示 */
#define M_ERROR_MESSAGE_OFF		0x00800000L	/* 〃を表示しない */
#define M_BAR_WINDOW_ON			0x01000000L
#define M_BAR_WINDOW_OFF		0x02000000L

#define EXTRACT_FOUND_FILE		0x40000000L
#define EXTRACT_NAMED_FILE		0x80000000L
#endif /* EXTRACT_FOUND_FILE */

#if !defined(ISARC_FUNCTION_START)
#define ISARC_FUNCTION_START			0
#define ISARC							0
#define ISARC_GET_VERSION				1
#define ISARC_GET_CURSOR_INTERVAL		2
#define ISARC_SET_CURSOR_INTERVAL		3
#define ISARC_GET_BACK_GROUND_MODE		4
#define ISARC_SET_BACK_GROUND_MODE		5
#define ISARC_GET_CURSOR_MODE			6
#define ISARC_SET_CURSOR_MODE			7
#define ISARC_GET_RUNNING				8

#define ISARC_CHECK_ARCHIVE				16
#define ISARC_CONFIG_DIALOG				17
#define ISARC_GET_FILE_COUNT			18
#define ISARC_QUERY_FUNCTION_LIST		19
#define ISARC_HOUT						20
#define ISARC_STRUCTOUT					21
#define ISARC_GET_ARC_FILE_INFO			22

#define ISARC_OPEN_ARCHIVE				23
#define ISARC_CLOSE_ARCHIVE				24
#define ISARC_FIND_FIRST				25
#define ISARC_FIND_NEXT					26
#define ISARC_EXTRACT					27
#define ISARC_ADD						28
#define ISARC_MOVE						29
#define ISARC_DELETE					30
#define ISARC_SETOWNERWINDOW			31	/* UnlhaSetOwnerWindow */
#define ISARC_CLEAROWNERWINDOW			32	/* UnlhaClearOwnerWindow */
#define ISARC_SETOWNERWINDOWEX			33	/* UnlhaSetOwnerWindowEx */
#define ISARC_KILLOWNERWINDOWEX			34	/* UnlhaKillOwnerWindowEx */

#define ISARC_GET_ARC_FILE_NAME			40
#define ISARC_GET_ARC_FILE_SIZE			41
#define ISARC_GET_ARC_ORIGINAL_SIZE		42
#define ISARC_GET_ARC_COMPRESSED_SIZE	43
#define ISARC_GET_ARC_RATIO				44
#define ISARC_GET_ARC_DATE				45
#define ISARC_GET_ARC_TIME				46
#define ISARC_GET_ARC_OS_TYPE			47
#define ISARC_GET_ARC_IS_SFX_FILE		48
#define ISARC_GET_FILE_NAME				57
#define ISARC_GET_ORIGINAL_SIZE			58
#define ISARC_GET_COMPRESSED_SIZE		59
#define ISARC_GET_RATIO					60
#define ISARC_GET_DATE					61
#define ISARC_GET_TIME					62
#define ISARC_GET_CRC					63
#define ISARC_GET_ATTRIBUTE				64
#define ISARC_GET_OS_TYPE				65
#define ISARC_GET_METHOD				66
#define ISARC_GET_WRITE_TIME			67
#define ISARC_GET_CREATE_TIME			68
#define ISARC_GET_ACCESS_TIME			69

#define ISARC_GET_ORIGINAL_SIZE_EX		85	/* UnlhaGetOriginalSizeEx */
#define ISARC_GET_COMPRESSED_SIZE_EX	86	/* UnlhaGetCompressedSizeEx */


#define ISARC_FUNCTION_END				100
#endif	/* ISARC_FUNCTION_START */

#ifndef FA_RDONLY
/* Attribute */
#define FA_RDONLY       0x01            /* Read only attribute */
#define FA_HIDDEN       0x02            /* Hidden file */
#define FA_SYSTEM       0x04            /* System file */
#define FA_LABEL        0x08            /* Volume label */
#define FA_DIREC        0x10            /* Directory */
#define FA_ARCH         0x20            /* Archive */
#endif

#ifndef ERROR_ARC_FILE_OPEN
/* WARNING */
#define ERROR_DISK_SPACE		0x8005
#define ERROR_READ_ONLY			0x8006
#define ERROR_USER_SKIP			0x8007
#define ERROR_UNKNOWN_TYPE		0x8008
#define ERROR_METHOD			0x8009
#define ERROR_PASSWORD_FILE		0x800A
#define ERROR_VERSION			0x800B
#define ERROR_FILE_CRC			0x800C
#define ERROR_FILE_OPEN			0x800D
#define ERROR_MORE_FRESH		0x800E
#define ERROR_NOT_EXIST			0x800F
#define ERROR_ALREADY_EXIST		0x8010

#define ERROR_TOO_MANY_FILES	0x8011

/* ERROR */
#define ERROR_MAKEDIRECTORY		0x8012
#define ERROR_CANNOT_WRITE		0x8013
#define ERROR_HUFFMAN_CODE		0x8014
#define ERROR_COMMENT_HEADER	0x8015
#define ERROR_HEADER_CRC		0x8016
#define ERROR_HEADER_BROKEN		0x8017
#define ERROR_ARC_FILE_OPEN		0x8018
#define ERROR_NOT_ARC_FILE		0x8019
#define ERROR_CANNOT_READ		0x801A
#define ERROR_FILE_STYLE		0x801B
#define ERROR_COMMAND_NAME		0x801C
#define ERROR_MORE_HEAP_MEMORY	0x801D
#define ERROR_ENOUGH_MEMORY		0x801E
#if !defined(ERROR_ALREADY_RUNNING)
#define ERROR_ALREADY_RUNNING	0x801F
#endif
#define ERROR_USER_CANCEL		0x8020
#define ERROR_HARC_ISNOT_OPENED	0x8021
#define ERROR_NOT_SEARCH_MODE	0x8022
#define ERROR_NOT_SUPPORT		0x8023
#define ERROR_TIME_STAMP		0x8024
#define ERROR_TMP_OPEN			0x8025
#define ERROR_LONG_FILE_NAME	0x8026
#define ERROR_ARC_READ_ONLY		0x8027
#define ERROR_SAME_NAME_FILE	0x8028
#define ERROR_NOT_FIND_ARC_FILE 0x8029
#define ERROR_RESPONSE_READ		0x802A
#define ERROR_NOT_FILENAME		0x802B
#define ERROR_TMP_COPY			0x802C
#define ERROR_EOF				0x802D
#define ERROR_ADD_TO_LARC		0x802E
#define ERROR_TMP_BACK_SPACE	0x802F
#define ERROR_SHARING			0x8030
#define ERROR_NOT_FIND_FILE		0x8031
#define ERROR_LOG_FILE			0x8032
#define	ERROR_NO_DEVICE			0x8033
#define ERROR_GET_ATTRIBUTES	0x8034
#define ERROR_SET_ATTRIBUTES	0x8035
#define ERROR_GET_INFORMATION	0x8036
#define ERROR_GET_POINT			0x8037
#define ERROR_SET_POINT			0x8038
#define ERROR_CONVERT_TIME		0x8039
#define ERROR_GET_TIME			0x803a
#define ERROR_SET_TIME			0x803b
#define ERROR_CLOSE_FILE		0x803c
#define ERROR_HEAP_MEMORY		0x803d
#define ERROR_HANDLE			0x803e
#define ERROR_TIME_STAMP_RANGE	0x803f

#define ERROR_END	ERROR_TIME_STAMP_RANGE
#endif /* ERROR_ARC_FILE_OPEN */

#define ARCHIVETYPE_NORMAL 0
#define ARCHIVETYPE_TAR 1
#define ARCHIVETYPE_TARGZ 2
#define ARCHIVETYPE_TARZ 3
#define ARCHIVETYPE_GZ 4
#define ARCHIVETYPE_Z 5
#define ARCHIVETYPE_TARBZ2 6
#define ARCHIVETYPE_BZ2	7

#define ARCHIVETYPE_CPIO	32
#define ARCHIVETYPE_CPIOGZ	32+4
#define ARCHIVETYPE_CPIOZ	32+5
#define ARCHIVETYPE_CPIOBZ2	32+7

#define ARCHIVETYPE_AR		48
#define ARCHIVETYPE_ARGZ	48+4
#define ARCHIVETYPE_ARZ		48+5
#define ARCHIVETYPE_ARBZ2	48+7

#endif // TAR32API_H

