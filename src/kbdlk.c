/***************************************************************************\
* Module Name: kbdlk.C
*
* keyboard layout
*
* Copyright (c) 1985-2001, Microsoft Corporation
*
* History:
* Based on KBDUS.C created by:
* KBDTOOL v3.40 - Created  Tue Oct 01 00:39:42 2019
\***************************************************************************/

#include <windows.h>
#include "kbdlk.h"

#if defined(_M_IA64)
#pragma section(".data")
#define ALLOC_SECTION_LDATA __declspec(allocate(".data"))
#else
#pragma data_seg(".data")
#define ALLOC_SECTION_LDATA
#endif
#else
#include "vkoem.h"
#define ALLOC_SECTION_LDATA
#endif

/***************************************************************************\
* ausVK[] - Virtual Scan Code to Virtual Key conversion table
\***************************************************************************/

static ALLOC_SECTION_LDATA USHORT ausVK[] = {
    T00, T01, T02, T03, T04, T05, T06, T07,
    T08, T09, T0A, T0B, T0C, T0D, T0E, T0F,
    T10, T11, T12, T13, T14, T15, T16, T17,
    T18, T19, T1A, T1B, T1C, T1D, T1E, T1F,
    T20, T21, T22, T23, T24, T25, T26, T27,
    T28, T29, T2A, T2B, T2C, T2D, T2E, T2F,
    T30, T31, T32, T33, T34, T35,

    /*
     * Right-hand Shift key must have KBDEXT bit set.
     */
    T36 | KBDEXT,

    T37 | KBDMULTIVK,               // numpad_* + Shift/Alt -> SnapShot

    T38, T39, T3A, T3B, T3C, T3D, T3E,
    T3F, T40, T41, T42, T43, T44,

    /*
     * NumLock Key:
     *     KBDEXT     - VK_NUMLOCK is an Extended key
     *     KBDMULTIVK - VK_NUMLOCK or VK_PAUSE (without or with CTRL)
     */
#if VK_PF1 == VK_NUMLOCK
    T45 | KBDEXT | KBDMULTIVK,
#else
    T45 | KBDEXT,
#endif

#if KBDLK_F18F19
    T46,
#else
    T46 | KBDMULTIVK,
#endif

    /*
     * Number Pad keys:
     *     KBDNUMPAD  - digits 0-9 and decimal point.
     *     KBDSPECIAL - require special processing by Windows
     */
    T47 | KBDNUMPAD | KBDSPECIAL,   // Numpad 7 (Home)
    T48 | KBDNUMPAD | KBDSPECIAL,   // Numpad 8 (Up),
    T49 | KBDNUMPAD | KBDSPECIAL,   // Numpad 9 (PgUp),
    T4A,
    T4B | KBDNUMPAD | KBDSPECIAL,   // Numpad 4 (Left),
    T4C | KBDNUMPAD | KBDSPECIAL,   // Numpad 5 (Clear),
    T4D | KBDNUMPAD | KBDSPECIAL,   // Numpad 6 (Right),
    T4E,
    T4F | KBDNUMPAD | KBDSPECIAL,   // Numpad 1 (End),
    T50 | KBDNUMPAD | KBDSPECIAL,   // Numpad 2 (Down),
    T51 | KBDNUMPAD | KBDSPECIAL,   // Numpad 3 (PgDn),
    T52 | KBDNUMPAD | KBDSPECIAL,   // Numpad 0 (Ins),
    T53 | KBDNUMPAD | KBDSPECIAL,   // Numpad . (Del),

    T54, T55, T56, T57, T58, T59, T5A, T5B,
    T5C, T5D, T5E, T5F, T60, T61, T62, T63,
    T64, T65, T66, T67, T68, T69, T6A, T6B,
    T6C, T6D, T6E, T6F, T70, T71, T72, T73,
    T74, T75, T76, T77, T78, T79, T7A, T7B,
    T7C, T7D, T7E

};

static ALLOC_SECTION_LDATA VSC_VK aE0VscToVk[] = {
        { 0x10, X10 | KBDEXT              },  // Speedracer: Previous Track
        { 0x19, X19 | KBDEXT              },  // Speedracer: Next Track
        { 0x20, X20 | KBDEXT              },  // Speedracer: Volume Mute
        { 0x21, X21 | KBDEXT              },  // Speedracer: Launch App 2
        { 0x22, X22 | KBDEXT              },  // Speedracer: Media Play/Pause
        { 0x24, X24 | KBDEXT              },  // Speedracer: Media Stop
        { 0x2E, X2E | KBDEXT              },  // Speedracer: Volume Down
        { 0x30, X30 | KBDEXT              },  // Speedracer: Volume Up
        { 0x32, X32 | KBDEXT              },  // Speedracer: Browser Home
#if KBDLK_LAYOUT == KBDLK_LK411
        { 0x0F, X0F | KBDEXT              },  // Compose
#endif
        { 0x1D, X1D | KBDEXT              },  // RControl (US) / RCompose (LK)
        { 0x35, X35 | KBDEXT              },  // Numpad Divide
        { 0x37, X37 | KBDEXT              },  // Snapshot
        { 0x38, X38 | KBDEXT              },  // RMenu
        { 0x47, X47 | KBDEXT              },  // Home
        { 0x48, X48 | KBDEXT              },  // Up
        { 0x49, X49 | KBDEXT              },  // Prior
        { 0x4B, X4B | KBDEXT              },  // Left
        { 0x4D, X4D | KBDEXT              },  // Right
        { 0x4F, X4F | KBDEXT              },  // End
        { 0x50, X50 | KBDEXT              },  // Down
        { 0x51, X51 | KBDEXT              },  // Next
        { 0x52, X52 | KBDEXT              },  // Insert
        { 0x53, X53 | KBDEXT              },  // Delete
        { 0x5B, X5B | KBDEXT              },  // Left Win
        { 0x5C, X5C | KBDEXT              },  // Right Win
        { 0x5D, X5D | KBDEXT              },  // Application
        { 0x5F, X5F | KBDEXT              },  // Speedracer: Sleep
        { 0x65, X65 | KBDEXT              },  // Speedracer: Browser Search
        { 0x66, X66 | KBDEXT              },  // Speedracer: Browser Favorites
        { 0x67, X67 | KBDEXT              },  // Speedracer: Browser Refresh
        { 0x68, X68 | KBDEXT              },  // Speedracer: Browser Stop
        { 0x69, X69 | KBDEXT              },  // Speedracer: Browser Forward
        { 0x6A, X6A | KBDEXT              },  // Speedracer: Browser Back
        { 0x6B, X6B | KBDEXT              },  // Speedracer: Launch App 1
        { 0x6C, X6C | KBDEXT              },  // Speedracer: Launch Mail
        { 0x6D, X6D | KBDEXT              },  // Speedracer: Launch Media Selector
        { 0x1C, X1C | KBDEXT              },  // Numpad Enter
#if !KBDLK_F18F19
        { 0x46, X46 | KBDEXT              },  // Break (Ctrl + Pause)
#endif
#if KBDLK_LAYOUT == KBDLK_LK411
        { 0x3D, X3D | KBDEXT              },  // F13
        { 0x3E, X3E | KBDEXT              },  // F14
        { 0x3F, X3F | KBDEXT              },  // F15 / Help
        { 0x40, X40 | KBDEXT              },  // F16 / Do
        { 0x41, X41 | KBDEXT              },  // F17
        { 0x4E, X4E | KBDEXT              },  // Num -
#endif
        { 0,      0                       }
};

static ALLOC_SECTION_LDATA VSC_VK aE1VscToVk[] = {
        { 0x1D, Y1D                       },  // Pause
        { 0   ,   0                       }
};

/***************************************************************************\
* aVkToBits[]  - map Virtual Keys to Modifier Bits
*
* See kbd.h for a full description.
*
* The keyboard has only three shifter keys:
*     SHIFT (L & R) affects alphabnumeric keys,
*     CTRL  (L & R) is used to generate control characters
*     ALT   (L & R) used for generating characters by number with numpad
\***************************************************************************/
static ALLOC_SECTION_LDATA VK_TO_BIT aVkToBits[] = {
    { VK_SHIFT    ,   KBDSHIFT     },
    { VK_CONTROL  ,   KBDCTRL      },
    { VK_MENU     ,   KBDALT       },
#if COMPOSE_TYPE == COMPOSE_MOD && !USE_ALTGR
    { VK_COMPOSE  ,   8            },
#endif
    { 0           ,   0           }
};

/***************************************************************************\
* aModification[]  - map character modifier bits to modification number
*
* See kbd.h for a full description.
*
\***************************************************************************/

static ALLOC_SECTION_LDATA MODIFIERS CharModifiers = {
    &aVkToBits[0],
#if USE_ALTGR
    6,
#elif COMPOSE_TYPE == COMPOSE_MOD
    8,
#else
    2,
#endif
    {
    //  Modification# //  Keys Pressed
    //  ============= // =============
        0,            // 
        1,            // Shift 
        2             // Control 
#if COMPOSE_TYPE == COMPOSE_MOD
       ,SHFT_INVALID  //       Alt
       ,SHFT_INVALID  // Shift+Alt
#if !USE_ALTGR
       ,SHFT_INVALID  //   Ctl+Alt
       ,SHFT_INVALID  // S+Ctl+Alt
#endif
       ,3             //       Ctl+Alt/      RAlt or       VK_COMPOSE
       ,4             // Shift+Ctl+Alt/Shift+RAlt or Shift+VK_COMPOSE
#endif
     }
};

/***************************************************************************\
*
* aVkToWch2[]  - Virtual Key to WCHAR translation for 2 shift states
* aVkToWch3[]  - Virtual Key to WCHAR translation for 3 shift states
* aVkToWch4[]  - Virtual Key to WCHAR translation for 4 shift states
*
* Table attributes: Unordered Scan, null-terminated
*
* Search this table for an entry with a matching Virtual Key to find the
* corresponding unshifted and shifted WCHAR characters.
*
* Special values for VirtualKey (column 1)
*     0xff          - dead chars for the previous entry
*     0             - terminate the list
*
* Special values for Attributes (column 2)
*     CAPLOK bit    - CAPS-LOCK affect this key like SHIFT
*
* Special values for wch[*] (column 3 & 4)
*     WCH_NONE      - No character
*     WCH_DEAD      - Dead Key (diaresis) or invalid (US keyboard has none)
*     WCH_LGTR      - Ligature (generates multiple characters)
*
\***************************************************************************/

#if COMPOSE_TYPE != COMPOSE_MOD
# define VK_TO_WCHARS2X VK_TO_WCHARS2
# define VK_TO_WCHARS3X VK_TO_WCHARS3
# define S2 2
# define S3 3
# define C2(VK,FL,U,S) \
  {VK   ,FL ,U        ,S        }
# define C3(VK,FL,U,S,C) \
  {VK   ,FL ,U        ,S        ,C        }
#define ZZ
#else
# define VK_TO_WCHARS2X VK_TO_WCHARS5
# define VK_TO_WCHARS3X VK_TO_WCHARS5
# define S2 5
# define S3 5
# define C2(VK,FL,U,S) \
  {VK   ,FL ,U        ,S        ,WCH_NONE ,WCH_DEAD ,WCH_DEAD}, \
  {0xff ,FL ,WCH_NONE ,WCH_NONE ,WCH_NONE ,U        ,S       }
# define C3(VK,FL,U,S,C) \
  {VK   ,FL ,U        ,S        ,C        ,WCH_DEAD ,WCH_DEAD}, \
  {0xff ,FL ,WCH_NONE ,WCH_NONE ,WCH_NONE ,U        ,S       }
#define ZZ ,0,0
#endif

static ALLOC_SECTION_LDATA VK_TO_WCHARS2X aVkToWch2[] = {
//                      |         |  Shift  |
//                      |=========|=========|
C2(VK_TAB       ,0      ,'\t'     ,'\t'     ),
C2(VK_ADD       ,0      ,'+'      ,'+'      ),
C2(VK_DIVIDE    ,0      ,'/'      ,'/'      ),
C2(VK_MULTIPLY  ,0      ,'*'      ,'*'      ),
C2(VK_SUBTRACT  ,0      ,'-'      ,'-'      ),
#if COMPOSE_TYPE == COMPOSE_DEAD
C2(VK_COMPOSE   ,0      ,WCH_DEAD ,WCH_DEAD ),
C2(0xff         ,0      ,COMPOSE  ,COMPOSE  ),
#ifdef VK_RCOMPOSE
C2(VK_RCOMPOSE  ,0      ,WCH_DEAD ,WCH_DEAD ),
C2(0xff         ,0      ,COMPOSE  ,COMPOSE  ),
#endif
#endif
  {0            ,0      ,0        ,0      ZZ}
};

static ALLOC_SECTION_LDATA VK_TO_WCHARS3 aVkToWch3[] = {
//                      |         |  Shift  |  Ctrl   |
//                      |=========|=========|=========|
C3('1'          ,0      ,'1'      ,'!'      ,WCH_NONE ),
C3('2'          ,0      ,'2'      ,'@'      ,WCH_NONE ),
C3('3'          ,0      ,'3'      ,'#'      ,WCH_NONE ),
C3('4'          ,0      ,'4'      ,'$'      ,WCH_NONE ),
C3('5'          ,0      ,'5'      ,'%'      ,WCH_NONE ),
C3('6'          ,0      ,'6'      ,'^'      ,WCH_NONE ),
C3('7'          ,0      ,'7'      ,'&'      ,WCH_NONE ),
C3('8'          ,0      ,'8'      ,'*'      ,WCH_NONE ),
C3('9'          ,0      ,'9'      ,'('      ,WCH_NONE ),
C3('0'          ,0      ,'0'      ,')'      ,WCH_NONE ),
C3(VK_OEM_MINUS ,0      ,'-'      ,'_'      ,WCH_NONE ),
C3(VK_OEM_PLUS  ,0      ,'='      ,'+'      ,WCH_NONE ),
C3('Q'          ,CAPLOK ,'q'      ,'Q'      ,WCH_NONE ),
C3('W'          ,CAPLOK ,'w'      ,'W'      ,WCH_NONE ),
C3('E'          ,CAPLOK ,'e'      ,'E'      ,WCH_NONE ),
C3('R'          ,CAPLOK ,'r'      ,'R'      ,WCH_NONE ),
C3('T'          ,CAPLOK ,'t'      ,'T'      ,WCH_NONE ),
C3('Y'          ,CAPLOK ,'y'      ,'Y'      ,WCH_NONE ),
C3('U'          ,CAPLOK ,'u'      ,'U'      ,WCH_NONE ),
C3('I'          ,CAPLOK ,'i'      ,'I'      ,WCH_NONE ),
C3('O'          ,CAPLOK ,'o'      ,'O'      ,WCH_NONE ),
C3('P'          ,CAPLOK ,'p'      ,'P'      ,WCH_NONE ),
C3(VK_OEM_4     ,0      ,'['      ,'{'      ,0x001b   ),
C3(VK_OEM_6     ,0      ,']'      ,'}'      ,0x001d   ),
C3('A'          ,CAPLOK ,'a'      ,'A'      ,WCH_NONE ),
C3('S'          ,CAPLOK ,'s'      ,'S'      ,WCH_NONE ),
C3('D'          ,CAPLOK ,'d'      ,'D'      ,WCH_NONE ),
C3('F'          ,CAPLOK ,'f'      ,'F'      ,WCH_NONE ),
C3('G'          ,CAPLOK ,'g'      ,'G'      ,WCH_NONE ),
C3('H'          ,CAPLOK ,'h'      ,'H'      ,WCH_NONE ),
C3('J'          ,CAPLOK ,'j'      ,'J'      ,WCH_NONE ),
C3('K'          ,CAPLOK ,'k'      ,'K'      ,WCH_NONE ),
C3('L'          ,CAPLOK ,'l'      ,'L'      ,WCH_NONE ),
C3(VK_OEM_1     ,0      ,';'      ,':'      ,WCH_NONE ),
C3(VK_OEM_7     ,0      ,'\''     ,'\"'     ,WCH_NONE ),
#if KBDLK_LAYOUT == KBDLK_LK411
C3(VK_OEM_3     ,0      ,'`'      ,'~'      ,0x001b   ),
#else
C3(VK_OEM_3     ,0      ,'`'      ,'~'      ,WCH_NONE ),
#endif
C3(VK_OEM_5     ,0      ,'\\'     ,'|'      ,0x001c   ),
C3('Z'          ,CAPLOK ,'z'      ,'Z'      ,WCH_NONE ),
C3('X'          ,CAPLOK ,'x'      ,'X'      ,WCH_NONE ),
C3('C'          ,CAPLOK ,'c'      ,'C'      ,WCH_NONE ),
C3('V'          ,CAPLOK ,'v'      ,'V'      ,WCH_NONE ),
C3('B'          ,CAPLOK ,'b'      ,'B'      ,WCH_NONE ),
C3('N'          ,CAPLOK ,'n'      ,'N'      ,WCH_NONE ),
C3('M'          ,CAPLOK ,'m'      ,'M'      ,WCH_NONE ),
#ifdef KBDLK_SHIFT_COMMA_PERIOD
C3(VK_OEM_COMMA ,0      ,','      ,','      ,WCH_NONE ),
#if defined(KBDLK_TEST) && defined(WCH_LGTR)
C3(VK_OEM_PERIOD,0      ,'.'      ,WCH_LGTR ,WCH_NONE ),
#else
C3(VK_OEM_PERIOD,0      ,'.'      ,'.'      ,WCH_NONE ),
#endif
#else
C3(VK_OEM_COMMA ,0      ,','      ,'<'      ,WCH_NONE ),
C3(VK_OEM_PERIOD,0      ,'.'      ,'>'      ,WCH_NONE ),
#endif
#if KBDLK_LAYOUT == KBDLK_LK411
C3(VK_OEM_102   ,0      ,'<'      ,'>'      ,WCH_NONE ),
#endif
C3(VK_OEM_2     ,0      ,'/'      ,'?'      ,WCH_NONE ),
C3(VK_SPACE     ,0      ,' '      ,' '      ,' '      ),
#if KBDLK_LAYOUT != KBDLK_LK411
C3(VK_OEM_102   ,0      ,'\\'     ,'|'      ,0x001c   ),
#endif
C3(VK_DECIMAL   ,0      ,'.'      ,'.'      ,WCH_NONE ),
C3(VK_BACK      ,0      ,'\b'     ,'\b'     ,0x007f   ),
C3(VK_ESCAPE    ,0      ,0x001b   ,0x001b   ,0x001b   ),
C3(VK_RETURN    ,0      ,'\r'     ,'\r'     ,'\n'     ),
C3(VK_CANCEL    ,0      ,0x0003   ,0x0003   ,0x0003   ),
  {0            ,0      ,0        ,0        ,0      ZZ}
};

// Put this last so that VkKeyScan interprets number characters
// as coming from the main section of the kbd (aVkToWch2 and
// aVkToWch5) before considering the numpad (aVkToWch1).

static ALLOC_SECTION_LDATA VK_TO_WCHARS1 aVkToWch1[] = {
    { VK_NUMPAD0   , 0      ,  '0'   },
    { VK_NUMPAD1   , 0      ,  '1'   },
    { VK_NUMPAD2   , 0      ,  '2'   },
    { VK_NUMPAD3   , 0      ,  '3'   },
    { VK_NUMPAD4   , 0      ,  '4'   },
    { VK_NUMPAD5   , 0      ,  '5'   },
    { VK_NUMPAD6   , 0      ,  '6'   },
    { VK_NUMPAD7   , 0      ,  '7'   },
    { VK_NUMPAD8   , 0      ,  '8'   },
    { VK_NUMPAD9   , 0      ,  '9'   },
    { 0            , 0      ,  '\0'  }
};

static ALLOC_SECTION_LDATA VK_TO_WCHAR_TABLE aVkToWcharTable[] = {
    {  (PVK_TO_WCHARS1)aVkToWch3,S3, sizeof(aVkToWch3[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch2,S2, sizeof(aVkToWch2[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch1, 1, sizeof(aVkToWch1[0]) },
    {                       NULL, 0, 0                    },
};

/***************************************************************************\
* aKeyNames[], aKeyNamesExt[]  - Virtual Scancode to Key Name tables
*
* Table attributes: Ordered Scan (by scancode), null-terminated
*
* Only the names of Extended, NumPad, Dead and Non-Printable keys are here.
* (Keys producing printable characters are named by that character)
\***************************************************************************/

static ALLOC_SECTION_LDATA VSC_LPWSTR aKeyNames[] = {
#if KBDLK_LAYOUT != KBDLK_LK411
    0x01,    L"Esc",
#endif
    0x0e,    L"Backspace",
    0x0f,    L"Tab",
    0x1c,    L"Enter",
    0x1d,    L"Ctrl",
    0x2a,    L"Shift",
    0x36,    L"Right Shift",
    0x37,    L"Num *",
    0x38,    L"Alt",
    0x39,    L"Space",
    0x3a,    L"Caps Lock",
    0x3b,    L"F1",
    0x3c,    L"F2",
    0x3d,    L"F3",
    0x3e,    L"F4",
    0x3f,    L"F5",
    0x40,    L"F6",
    0x41,    L"F7",
    0x42,    L"F8",
    0x43,    L"F9",
    0x44,    L"F10",
    0x45,    L"Pause",
#if KBDLK_F18F19
    0x46,    L"F19",
#else
    0x46,    L"Scroll Lock",
#endif
    0x47,    L"Num 7",
    0x48,    L"Num 8",
    0x49,    L"Num 9",
#if KBDLK_LAYOUT == KBDLK_LK411
    0x4a,    L"Num +",
#else
    0x4a,    L"Num -",
#endif
    0x4b,    L"Num 4",
    0x4c,    L"Num 5",
    0x4d,    L"Num 6",
#if KBDLK_LAYOUT == KBDLK_LK411
    0x4e,    L"Num ,",
#else
    0x4e,    L"Num +",
#endif
    0x4f,    L"Num 1",
    0x50,    L"Num 2",
    0x51,    L"Num 3",
    0x52,    L"Num 0",
    0x53,    L"Num Del",
    0x54,    L"Sys Req",
#if KBDLK_LAYOUT == KBDLK_LK411
    0x57,    L"Esc",
#else
    0x57,    L"F11",
#endif
    0x58,    L"F12",
    0x7c,    L"F13",
    0x7d,    L"F14",
    0x7e,    L"F15",
    0x7f,    L"F16",
    0x80,    L"F17",
    0x81,    L"F18",
    0x82,    L"F19",
    0x83,    L"F20",
    0x84,    L"F21",
    0x85,    L"F22",
    0x86,    L"F23",
    0x87,    L"F24",
    0   ,    NULL
};

static ALLOC_SECTION_LDATA VSC_LPWSTR aKeyNamesExt[] = {
#ifdef VS_COMPOSE
    VS_COMPOSE, L"Compose",
#endif
#ifdef VS_RCOMPOSE
    VS_RCOMPOSE, L"Right Compose",
#endif
    0x1c,    L"Num Enter",
    0x1d,    L"Right Ctrl",
    0x35,    L"Num /",
#if KBDLK_F18F19
    0x37,    L"F18",
#else
    0x37,    L"Prnt Scrn",
#endif
#if defined(VS_COMPOSE) && VS_COMPOSE != 0x38
    0x38,    L"Right Alt",
#endif
#if KBDLK_LAYOUT == KBDLK_LK411
    0x3D,    L"F13",
    0x3E,    L"F14",
    0x3F,    L"Help",
    0x40,    L"Do",
    0x41,    L"F17",
#endif
#if VK_PF1 == VK_NUMLOCK
    0x45,    L"Num Lock",
#else
    0x45,    L"PF1",
#endif
    0x46,    L"Break",
    0x47,    L"Home",
    0x48,    L"Up",
    0x49,    L"Page Up",
    0x4b,    L"Left",
    0x4d,    L"Right",
#if KBDLK_LAYOUT == KBDLK_LK411
    0x4e,    L"Num -",
#endif
    0x4f,    L"End",
    0x50,    L"Down",
    0x51,    L"Page Down",
    0x52,    L"Insert",
    0x53,    L"Delete",
    0x54,    L"<00>",
    0x56,    L"Help",
    0x5b,    L"Left Windows",
    0x5c,    L"Right Windows",
    0x5d,    L"Application",
    0   ,    NULL
};

#if COMPOSE_TYPE == COMPOSE_DEAD

static ALLOC_SECTION_LDATA LPWSTR aKeyNamesDead[] = {
    COMPOSE_STR L"Compose",
    NULL
};

// Use COMPOSE1 (once) and COMPOSE2 (for each sequence) when the first key
// is common (eg. e' and e`).

#define COMPOSE1(K1) \
    DEADTRANS(K1, COMPOSE, K1, DKF_DEAD)
#define COMPOSE2(K1,K2,WC) \
    DEADTRANS(K2, K1, WC, 0x0000)
#define COMPOSEX(K1,K2,WC) \
    COMPOSE1(K1), COMPOSE2(K1, K2, WC)

static ALLOC_SECTION_LDATA DEADKEY aDeadKey[] = {
#include "compose1.h"
#include "compose2.h"
    0, 0
};
#endif



#if COMPOSE_TYPE == COMPOSE_MOD
static ALLOC_SECTION_LDATA LPWSTR aKeyNamesDead[] = {
  NULL
};

#define COMPOSE2(K1,K2,WC) DEADTRANSX(K2,K1,WC)
static ALLOC_SECTION_LDATA DEADKEY aDeadKey[] = {
#include "compose2.h"
  0, 0
};
#endif



#if defined(KBDLK_TEST) && defined(WCH_LGTR)
TYPEDEF_LIGATURE(KBDLK_VSTR_LEN)
TYPEDEF_LIGATUREX(LIGATUREX,KBDLK_VSTR_LEN) // LIGATUREX, *PLIGATUREX;
static ALLOC_SECTION_LDATA LIGATUREX aLigature[] = {
  { VK_OEM_PERIOD, 1, KBDLK_VSTR },
  { 0, 0, 0 }
};
#endif

// Crashes Windows!!
// #pragma data_seg("my_data")

static ALLOC_SECTION_LDATA KBDTABLES KbdTables = {
    /*
     * Modifier keys
     */
    &CharModifiers,

    /*
     * Characters tables
     */
    aVkToWcharTable,

    /*
     * Diacritics
     */
#if COMPOSE_TYPE == COMPOSE_DEAD || COMPOSE_TYPE == COMPOSE_MOD
    aDeadKey,
#else
    NULL,
#endif

    /*
     * Names of Keys
     */
    aKeyNames,
    aKeyNamesExt,
#if COMPOSE_TYPE == COMPOSE_DEAD || COMPOSE_TYPE == COMPOSE_MOD
    aKeyNamesDead,
#else
    NULL,
#endif

    /*
     * Scan codes to Virtual Keys
     */
    ausVK,
    sizeof(ausVK) / sizeof(ausVK[0]),
    aE0VscToVk,
    aE1VscToVk,

    /*
     * Locale-specific special processing
     */
    MAKELONG(USE_ALTGR, KBD_VERSION),

    /*
     * Ligatures
     */
#if defined(KBDLK_TEST) && defined(WCH_LGTR)
    KBDLK_VSTR_LEN,
    sizeof(aLigature[0]),
    (PLIGATURE1) aLigature
#else
    0,
    0,
    NULL
#endif
};

#ifdef KBDLK_TEST
void MyReportEvent(DWORD id, LPSTR szMsg)
{
    HANDLE h; 
 
    h = RegisterEventSource(NULL,       // uses local computer 
			    "kbdlk");   // source name 
    if (h == NULL) return;
 
    ReportEvent(h,                      // event log handle 
		EVENTLOG_SUCCESS,       // event type 
		0,                      // category zero 
		id,                     // event identifier 
		NULL,                   // no user security identifier 
		1,                      // one substitution string 
		0,                      // no data 
		(LPTSTR *) &szMsg,      // pointer to string array 
		NULL);                  // pointer to data 
 
    DeregisterEventSource(h); 
}
#endif

PKBDTABLES KbdLayerDescriptor(VOID)
{
#ifdef KBDLK_TEST
  MyReportEvent (0, "kbdlk started: " STR(KBDLK_VERSION) " "
		 __DATE__ " " __TIME__);
#endif
    return &KbdTables;
}
