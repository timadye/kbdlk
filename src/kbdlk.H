/****************************** Module Header ******************************\
* Module Name: kbdlk.H
*
* keyboard layout header
*
* Copyright (c) 1985-2001, Microsoft Corporation
*
* Various defines for use by keyboard input code.
*
* History:
* Based on KBDUS.C created by:
* created by KBDTOOL v3.40 Tue Oct 01 00:39:42 2019
*
\***************************************************************************/

#include "version.h"

/*
 * kbd type should be controlled by cl command-line argument
 */
#define KBD_TYPE 4

/*
* Include the basis of all keyboard table values
*/
#include "kbd.h"

//#define KBDLK_TEST   1

#define KBDLK_US     0
#define KBDLK_LK411  1

#define COMPOSE_NONE 0
#define COMPOSE_DEAD 1
#define COMPOSE_MOD  2

#define KBDLK_LAYOUT KBDLK_LK411

#if KBDLK_LAYOUT == KBDLK_LK411
# define KBDLK_SHIFT_COMMA_PERIOD 1
# define KBDLK_F18F19 1
# define KBDLK_USBCONV 1
#else
# undef  KBDLK_SHIFT_COMMA_PERIOD
# undef  KBDLK_F18F19
# undef  KBDLK_USBCONV
#endif

#ifdef DKF_DEAD
// COMPOSE_DEAD would be better, as it is like the traditional DEC behaviour
// and can also look like a modifier key. Unfortunately the repeat function
// causes lots of Composes to be inserted if it is held down too long. Bum.
# define COMPOSE_TYPE COMPOSE_MOD   // COMPOSE_DEAD
# define DEADTRANSX(K2,K1,WC) DEADTRANS(K2,K1,WC,0x0000)
#else
# define COMPOSE_TYPE COMPOSE_MOD
# define DEADTRANSX(K2,K1,WC) DEADTRANS(K2,K1,WC)
#endif

#if COMPOSE_TYPE != COMPOSE_NONE
# if KBDLK_LAYOUT == KBDLK_LK411
#   define VK_COMPOSE  0xD8
# else
#   define VK_COMPOSE  VK_RMENU
# endif
# define COMPOSE       0x00a0    // any unused Unicode value
# define COMPOSE_STR L"\x00a0"
#else
# undef  VK_COMPOSE
# undef  COMPOSE
# undef  COMPOSE_STR
#endif

#if COMPOSE_TYPE == COMPOSE_MOD && VK_COMPOSE == VK_RMENU
# define USE_ALTGR KLLF_ALTGR
#else
# define USE_ALTGR 0
#endif


#define LLSTR(x) L ## x
#define LSTR(x) LLSTR(x)
#define LLQSTR(x) LSTR(#x)
#define LQSTR(x) LLQSTR(x)
#define SSTR(x) #x
#define STR(x) SSTR(x)
#define TYPEDEF_LIGATUREX(x,n) \
  typedef LIGATURE##n x; \
  typedef PLIGATURE##n P##x;


#define KBDLK_VSTR (LQSTR(KBDLK_VERSION) L" " LSTR(__TIME__))
#define KBDLK_VSTR_LEN (sizeof(KBDLK_VSTR)/sizeof(wchar_t)-1)




/***************************************************************************\
* The table below defines the virtual keys for various keyboard types where
* the keyboard differ from the US keyboard.
*
* _EQ() : all keyboard types have the same virtual key for this scancode
* _NE() : different virtual keys for this scancode, depending on kbd type
*
*     +------+ +----------+----------+----------+----------+----------+----------+
*     | Scan | |    kbd   |    kbd   |    kbd   |    kbd   |    kbd   |    kbd   |
*     | code | |   type 1 |   type 2 |   type 3 |   type 4 |   type 5 |   type 6 |
\****+-------+_+----------+----------+----------+----------+----------+----------+*/
#if   KBDLK_LAYOUT == KBDLK_US

# undef  X38
# define X38 _EQ(       COMPOSE           )

#elif KBDLK_LAYOUT == KBDLK_LK411
#undef   T01
#define  T01 _EQ(       OEM_3             ) // `~
#undef   T29
#define  T29 _EQ(       OEM_102           ) // <>
#undef   T57
#define  T57 _EQ(       ESCAPE            )

# undef  X0F
# define X0F _EQ(       COMPOSE           )
# undef  X1D
# define X1D _EQ(       COMPOSE           )

#undef  T4A
#define T4A _EQ(        ADD               ) // PF4
#undef  X4E
#define X4E _EQ(        SUBTRACT          ) // KP_MINUS
#undef  T4E
#define T4E _EQ(        SEPARATOR         ) // KP_COMMA
#undef  X3D
#define X3D _EQ(        F13               )
#undef  X3E
#define X3E _EQ(        F14               )
#undef  X3F
#define X3F _EQ(        HELP              ) // Help : F15
#undef  X40
#define X40 _EQ(        APPS              ) // Do :   F16
#undef  X41
#define X41 _EQ(        F17               )

#ifdef KBDLK_F18F19
#undef  X37
#define X37 _EQ(        F18               )
#undef  T46
#define T46 _EQ(        F19               )
#undef  X46
#define X46 _EQ(        SCROLL            )
#endif

#ifdef KBDLK_USBCONV
#undef  T66
#define T66 _EQ(        HELP              ) // Help : F15
#undef  T67
#define T67 _EQ(        APPS              ) // Do :   F16
#undef  T7E
#define T7E _EQ(        SUBTRACT          ) // KP_MINUS
#undef  X5B
#define X5B _EQ(        COMPOSE           )
#endif

#endif
