#include <X11/XF86keysym.h>
#include "reload.h"

static const unsigned int borderpx  = 3;
static const unsigned int snap  = 32;
static const int showbar  = 1;
static const int topbar  = 1;
static const char *fonts[]  = { "Ubuntu Mono:bold:size=13" };
static const char dmenufont[]  = "Ubuntu Mono:bold:size=13";
/*static const char col_gray1[]  = "#222222";
static const char col_gray2[]  = "#444444";
static const char col_gray3[]  = "#bbbbbb";
static const char col_gray4[]  = "#eeeeee";
static const char col_cyan[]  = "#005577";*/
static const char col_gray1[]  = "#222222";
static const char col_gray2[]  = "#777777";
static const char col_gray3[]  = "#777777";
static const char col_gray4[]  = "#eeeeee";
static const char col_cyan[]  = "#222222";
static const char col_selected[]  = "#005577";
static const char *colors[][3]   = {
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel] =  { col_gray4, col_cyan,  col_selected  },
};

static const char *tags[] = { "www", "mail", "office", "code", "media", "rdp:srv", "rdp:204", "rdp:205" };

static const Rule rules[] = {
	{ "Urxvt",  NULL,  NULL,  0,  0,  -1 },
	{ "Zenmap",  NULL,  NULL,  0,  0,  -1 },
	{ "Wireshark-gtk",  NULL,  NULL,  0,  0,  -1 },
	{ "lxqt-sudo",  NULL,  NULL,  0,  1,  -1 },
	{ "Nautilus",  NULL,  NULL,  0,  1,  -1 },
	{ "Thunar",  NULL,  NULL,  0,  1,  -1 },
	{ "Obconf",  NULL,  NULL,  0,  1,  -1 },
	{ "Obmenu",  NULL,  NULL,  0,  1,  -1 },
	{ "Firefox",  NULL,  NULL,  1 << 0,  0,  -1 },
	{ "Chromium",  NULL,  NULL,  1 << 0,  0,  -1 },
	{ "Thunderbird",  NULL,  NULL,  1 << 1,  0,  -1 },
	{ "Epdfview",  NULL,  NULL,  1 << 2,  0,  -1 },
	{ "Xpdf",  NULL,  NULL,  1 << 2,  0,  -1 },
	{ "Zathura",  NULL,  NULL,  1 << 2,  0,  -1 },
	{ "Libreoffice",  NULL,  NULL,  1 << 2,  0,  -1 },
	{ "Geany",  NULL,  NULL,  1 << 3,  0,  -1 },
	{ "Scribus",  NULL,  NULL,  1 << 3,  0,  -1 },
	{ "sublime_text",  NULL,  NULL,  1 << 3,  0,  -1 },
	{ "Atom",  NULL,  NULL,  1 << 3,  0,  -1 },
	{ "processing-app-base", "sun-awt-X11-XFramePeer", NULL,  1 << 3,  0,  -1 },
	{ "octave-gui",  "octave-gui",  NULL,  1 << 3,  1,  -1 },
	{ "FreeMat",  "FreeMat",  NULL,  1 << 3,  0,  -1 },
	{ "Gimp",  NULL,  NULL,  1 << 4,  0,  -1 },
	{ "Gpicview",  NULL,  NULL,  1 << 4,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "perun",  1 << 5,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-1",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-2",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-3",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-4",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-5",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-6",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-7",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-8",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "204-9",  1 << 6,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "205-2",  1 << 7,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "205-3",  1 << 7,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "205-4",  1 << 7,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "205-5",  1 << 7,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "205-6",  1 << 7,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "205-7",  1 << 7,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "205-8",  1 << 7,  0,  -1 },
	{ "xfreerdp",  "xfreerdp",  "205-9",  1 << 7,  0,  -1 },
};

static const float mfact     = 0.50;
static const int nmaster     = 1;
static const int resizehints = 0;

static const Layout layouts[] = {
	{ "===",  bstackhoriz },
	{ "TTT",  bstack },
	{ "[]=",  tile },
	{ "><>",  NULL },
	{ "[M]",  monocle },
};

#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,  KEY,  view,  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,  KEY,  toggleview,  {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,   KEY,  tag,  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask,  KEY,  toggletag,  {.ui = 1 << TAG} },

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static char dmenumon[2] = "0";
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *usertermcmd[]  = { "urxvtc", NULL };
static const char *www1[] = { "firefox", NULL };
static const char *www2[] = { "chromium", NULL };
static const char *mail1[] = { "thunderbird", NULL };
static const char *numerics1[] = { "FreeMat", NULL };
static const char *numerics2[] = { "octave", "--force-gui", NULL };
static const char *pdf1[] = { "epdfview", NULL };
static const char *pdf2[] = { "xpdf", NULL };
static const char *editor1[] = { "libreoffice", NULL };
static const char *editor2[] = { "geany", NULL };
static const char *editor3[] = { "atom", NULL };
static const char *editor4[] = { "scribus", NULL };
static const char *editor5[] = { "subl", NULL };
static const char *img1[] = { "gpicview", NULL };
static const char *img2[] = { "gimp", NULL };
static const char *img3[] = { "inkscape", NULL };
static const char *schema1[] = { "dia", NULL };
static const char *schema2[] = { "yed", NULL };
static const char *schema3[] = { "eagle", NULL };
static const char *schema4[] = { "fritzing", NULL };
static const char *schema5[] = { "kicad", NULL };
static const char *fm1[] = { "dbus-launch", "Thunar", NULL };
static const char *rdp1[] = { "zdalny.sh", "wlk", NULL };
static const char *rdp2[] = { "zdalny.sh", "perun", NULL };
static const char *rdp3[] = { "zdalny.sh", "204", NULL };
static const char *rdp4[] = { "zdalny.sh", "205", NULL };
static const char *vol1[] = { "mixer", "-S", "vol", "+3", NULL };
static const char *vol2[] = { "mixer", "-S", "vol", "-3", NULL };
static const char *radio1[] = { "radio.sh", "off", NULL };
static const char *radio2[] = { "radio.sh", "antyradio", NULL };
static const char *mksshot[] = { "Zrzut.ekranu.sh", NULL};
static const char *labcheck[] = { "Hostcheck.sh", "lab", ">/dev/null", NULL };
static const char *srvcheck[] = { "Hostcheck.sh", "srv", ">/dev/null", NULL };
static const char *mpprev[] = { "mocp", "--previous", ">/dev/null", NULL };
static const char *mpplay[] = { "mocp", "--play", ">/dev/null", NULL };
static const char *mppause[] = { "mocp", "--toggle-pause", NULL };
static const char *mpstop[] = { "mocp", "--stop", ">/dev/null", NULL };
static const char *mpnext[] = { "mocp", "--next", ">/dev/null", NULL };
static const char *mpplayer[] = { "urxvtc", "-T", "MOCP", "-e", "mocp", NULL };

static Key keys[] = {
	{ MODKEY,  XK_j,  focusstack,  {.i = +1 } },
	{ MODKEY,  XK_a,  focusstack,  {.i = -1 } },
	{ MODKEY,  XK_k,  focusstack,  {.i = -1 } },
	{ MODKEY,  XK_i,  incnmaster,  {.i = +1 } },
	{ MODKEY,  XK_d,  incnmaster,  {.i = -1 } },
	{ MODKEY,  XK_h,  setmfact,  {.f = -0.05} },
	{ MODKEY,  XK_l,  setmfact,  {.f = +0.05} },
	{ MODKEY,  XK_Return,  zoom,  {0} },
	{ MODKEY,  XK_Tab,  view,  {0} },
	{ MODKEY,  XK_b,  setlayout,  {.v = &layouts[0]} },
	{ MODKEY,  XK_h,  setlayout,  {.v = &layouts[1]} },
	{ MODKEY,  XK_t,  setlayout,  {.v = &layouts[2]} },
	{ MODKEY,  XK_f,  setlayout,  {.v = &layouts[3]} },
	{ MODKEY,  XK_m,  setlayout,  {.v = &layouts[4]} },
	{ MODKEY,  XK_space,  setlayout,  {0} },
	{ MODKEY|ShiftMask,  XK_space,  togglefloating, {0} },
	{ MODKEY,  XK_0,  view,  {.ui = ~0 } },
	{ MODKEY|ShiftMask,  XK_0,  tag,  {.ui = ~0 } },
	{ MODKEY,  XK_comma,  focusmon,  {.i = -1 } },
	{ MODKEY,  XK_period,  focusmon,  {.i = +1 } },
	{ MODKEY|ShiftMask,  XK_comma,  tagmon,  {.i = -1 } },
	{ MODKEY|ShiftMask,  XK_period,  tagmon,  {.i = +1 } },
	TAGKEYS(XK_1,  0)
	TAGKEYS(XK_2,  1)
	TAGKEYS(XK_3,  2)
	TAGKEYS(XK_4,  3)
	TAGKEYS(XK_5,  4)
	TAGKEYS(XK_6,  5)
	TAGKEYS(XK_7,  6)
	TAGKEYS(XK_8,  7)
	TAGKEYS(XK_9,  8)
	{ MODKEY,  XK_p,  spawn,  {.v = dmenucmd } },
	{ MODKEY|ShiftMask,   XK_Return, spawn,  {.v = usertermcmd } },
	{ MODKEY|ShiftMask,   XK_f,  spawn,  {.v = www1 }},
	{ MODKEY|ControlMask,  XK_f,  spawn,  {.v = numerics1 }},
	{ MODKEY|ControlMask,  XK_o,  spawn,  {.v = numerics2 }},
	{ MODKEY|ShiftMask,  XK_l,  spawn,  {.v = editor1 }},
	{ MODKEY|ShiftMask,  XK_a,  spawn,  {.v = editor2 }},
	{ MODKEY|ControlMask,  XK_s,  spawn,  {.v = editor4 }},
	{ MODKEY|ShiftMask,  XK_s,  spawn,  {.v = editor5 }},
	{ MODKEY|ShiftMask,	 XK_m,  spawn,  {.v = mail1 }},
	{ MODKEY|ShiftMask,	 XK_e,  spawn,  {.v = pdf1 }},
	{ MODKEY|ShiftMask,  XK_g,  spawn,  {.v = img1 }},
	{ MODKEY|ControlMask,  XK_g,  spawn,  {.v = img2 }},
	{ MODKEY|ShiftMask,  XK_w,  spawn,  {.v = fm1 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_F1,  spawn,  {.v = rdp1 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_F2,  spawn,  {.v = rdp2 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_F4,  spawn,  {.v = rdp3 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_F5,  spawn,  {.v = rdp4 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_Left,  spawn,  {.v = radio1 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_Right,  spawn,  {.v = radio2 }},
	{ MODKEY,  XK_Up,  spawn,  {.v = vol1 }},
	{ MODKEY,  XK_Down,  spawn,  {.v = vol2 }},
	{ MODKEY,  XK_z,  spawn,  {.v = mpprev }},
	{ MODKEY,  XK_x,  spawn,  {.v = mpplay }},
	{ MODKEY|ShiftMask,  XK_x,  spawn,  {.v = mppause }},
	{ MODKEY|ControlMask,  XK_x,  spawn,  {.v = mpstop }},
	{ MODKEY,  XK_v,  spawn,  {.v = mpnext }},
	{ MODKEY|ControlMask|ShiftMask,  XK_n,  spawn,  {.v = mpplayer }},
	{ MODKEY|ControlMask|ShiftMask,  XK_d,  spawn,  {.v = schema1 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_y,  spawn,  {.v = schema2 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_e,  spawn,  {.v = schema3 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_f,  spawn,  {.v = schema4 }},
	{ MODKEY|ControlMask|ShiftMask,  XK_F11,  spawn, {.v = labcheck }},
	{ MODKEY|ControlMask|ShiftMask,  XK_F12,  spawn, {.v = srvcheck }},
	{ 0,  XK_Print,  spawn,  {.v = mksshot }},
	{ MODKEY,  XK_q,  killclient,  {0} },
	{ MODKEY|ControlMask,  XK_Escape,  quit,  {0} },
	{ MODKEY|ShiftMask,  XK_Escape,  reload,  {0} },
};

static Button buttons[] = {
	{ ClkLtSymbol,  0,  Button1,  setlayout,  {0} },
	{ ClkLtSymbol,  0,  Button3,  setlayout,  {.v = &layouts[2]} },
	{ ClkWinTitle,  0,  Button2,  zoom,  {0} },
	{ ClkStatusText,  0,  Button2,  spawn,  {.v = usertermcmd } },
	{ ClkClientWin,  MODKEY,  Button1,  movemouse,  {0} },
	{ ClkClientWin,  MODKEY,  Button2,  togglefloating, {0} },
	{ ClkClientWin,  MODKEY,  Button3,  resizemouse,  {0} },
	{ ClkTagBar,  0,  Button1,  view,  {0} },
	{ ClkTagBar,  0,  Button3,  toggleview,  {0} },
	{ ClkTagBar,  MODKEY,  Button1,  tag,  {0} },
	{ ClkTagBar,  MODKEY,  Button3,  toggletag,  {0} },
};
