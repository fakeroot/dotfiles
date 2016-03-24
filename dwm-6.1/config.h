#include <X11/XF86keysym.h>

static void reexec(const Arg *arg);

void reexec(const Arg *arg) {
  cleanup();
  XCloseDisplay(dpy);
  execvp(((char **)arg->v)[0], (char **)arg->v);
};

/* appearance */
static const char *fonts[] = {
   "dejavu sans mono:medium:pixelsize=15:medium:antialias=true"
	//"monospace:size=10"
};
//static const char dmenufont[] = "monospace:size=10";
static const char dmenufont[] = "dejavu sans mono:medium:pixelsize=15:medium:antialias=true";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[] = "#222222";
static const char normfgcolor[] = "#bbbbbb";
static const char selbordercolor[] = "#005577";
static const char selbgcolor[] = "#005577";
static const char selfgcolor[] = "#eeeeee";
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int snap = 32; /* snap pixel */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1; /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "term", "code", "media", "rdp:srv", "rdp:lab204", "rdp:lab205", "office", "mail", "www" };

static const Rule rules[] = {
/* xprop(1):
 *	WM_CLASS(STRING) = instance, class
 *	WM_NAME(STRING) = title
 */

/* class, instance, title, tags mask, isfloating, monitor */
   //{ "", NULL, NULL, 0, 1 << 0, NULL, -1 },
  { "gmrun", NULL, NULL, 0, 1, -1},
  { "Nautilus", NULL, NULL, 0 , 1, -1 },
  { "URxvt", NULL, NULL, 1 << 0, 0, -1 },
  { "Geany", NULL, NULL, 1 << 1, 0, -1 },   
  { "codeblocks", NULL, NULL, 1 << 1, 0, -1 },   
  { "URxvt", NULL, "MOCP", 1 << 2, 0, -1 },
  { "Gimp", NULL, NULL, 1 << 2, 0, -1 },
  { "gpicview", NULL, NULL, 1 << 2, 0, -1 },
  { "Vncviewer", NULL, NULL, 1 << 3, 1, -1 },
  { "rdesktop", NULL, "PERUN", 1 << 3, 1, -1 },
  { "rdesktop", NULL, "204-1", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "204-2", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "204-3", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "204-4", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "204-5", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "204-6", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "204-7", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "204-8", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "204-9", 1 << 4, 1, -1 },
  { "rdesktop", NULL, "205-1", 1 << 5, 1, -1 },
  { "rdesktop", NULL, "205-2", 1 << 5, 1, -1 },
  { "rdesktop", NULL, "205-3", 1 << 5, 1, -1 },
  { "rdesktop", NULL, "205-4", 1 << 5, 1, -1 },
  { "rdesktop", NULL, "205-5", 1 << 5, 1, -1 },
  { "rdesktop", NULL, "205-6", 1 << 5, 1, -1 },
  { "rdesktop", NULL, "205-7", 1 << 5, 1, -1 },
  { "rdesktop", NULL, "205-8", 1 << 5, 1, -1 },
  { "rdesktop", NULL, "205-9", 1 << 5, 1, -1 },
  { "libreoffice", NULL, NULL, 1 << 6, 0, -1 },
  { "dia", NULL, NULL, 1 << 6, 1, -1 },
  { "kicad", NULL, NULL, 1 << 6, 1, -1 },
  { "sun-awt-X11-XFramePeer", NULL, NULL, 1 << 6, 0, -1 },
  { "Thunderbird", "Mail", NULL, 1 << 7, 0, -1 },
  { "Thunderbird", "Msgcompose", NULL, 1 << 7, 1, -1 },
  { "Firefox", NULL, NULL, 1 << 8, 0, -1 },
  { "chromium-browser", NULL, NULL, 1 << 8, 0, -1 },   
};


/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol, arrange function */
	{ "[]=", tile },    /* first entry is default */
	{ "><>", NULL },    /* no layout function means floating behavior */
	{ "[M]", monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY, KEY, view, {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask, KEY, toggleview, {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask, KEY, tag, {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY, toggletag, {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *TermCmd1[] = { "urxvtc", "-title", "USER", NULL };
static const char *TermCmd2[] = { "urxvtc", "-title", "ROOT", "-e", "su", "-", NULL };
static const char *Browser1[] = { "chrome", NULL };
static const char *Browser2[] = { "firefox", NULL };
static const char *MailClient1[] = { "thunderbird", NULL };
static const char *MailClient2[] = { "claws-mail", NULL };
static const char *Editor1[] = { "geany", NULL };
static const char *Editor2[] = { "libreoffice", NULL };
static const char *Schematics1[] = { "dia", NULL };
static const char *Schematics2[] = { "kicad", NULL };
static const char *Schematics3[] = { "java", "-jar", "/usr/home/f1y/.software/yed/yed.jar", NULL };
static const char *PdfViewer1[] = { "xpdf", NULL };
static const char *PdfViewer2[] = { "epdfview", NULL };
static const char *PdfViewer3[] = { "zathura", NULL };
static const char *Graphics1[] = { "gpicview", NULL };
static const char *Graphics2[] = { "gimp", NULL };
static const char *Graphics3[] = { "inkscape", NULL };
static const char *Graphics4[] = { "gqview", NULL };
static const char *FileManager1[] = { "nautilus", NULL };
static const char *FileManager2[] = { "Thunar", NULL };
static const char *FileManager3[] = { "pcmanfm", NULL };
static const char *Network1[] = { "wireshark", NULL };
static const char *Network2[] = { "zenmap", NULL };
static const char *Archiver1[] = { "xarchive", NULL };
static const char *Archiver2[] = { "xarchiver", NULL };
static const char *CommandRun[] = { "gmrun", NULL };
static const char *RdpWe[] = { "zdalny_pulpit.sh", "we", NULL };
static const char *RdpPerun[] = { "zdalny_pulpit.sh", "perun", NULL };
static const char *RdpAll[] = { "zdalny_pulpit.sh", "all", NULL };
static const char *Rdp204[] = { "zdalny_pulpit.sh", "204", NULL };
static const char *Rdp205[] = { "zdalny_pulpit.sh", "205", NULL };
static const char *CheckSrv[] = { "hostcheck.sh", "srv", NULL };
static const char *CheckPC[] = { "hostcheck.sh", "pc", NULL };
static const char *MusicPrevious[] = { "mocp", "--previous", ">/dev/null", NULL };
static const char *MusicPlay[] = { "mocp", "--play", ">/dev/null", NULL };
static const char *MusicTogglePause[] = { "mocp", "--toggle-pause", ">/dev/null", NULL };
static const char *MusicStop[] = { "mocp", "--stop", ">/dev/null", NULL };
static const char *MusicNext[] = { "mocp", "--next", ">/dev/null", NULL };
static const char *MusicPlayerRun[] = { "urxvtc", "-title", "MOCP", "-e", "mocp", NULL };

static Key keys[] = {
	/* modifier, key, function, argument */
  { MODKEY, XK_p, spawn, {.v = dmenucmd } },
  { MODKEY|ShiftMask, XK_Return, spawn, {.v = TermCmd1 } },
  { MODKEY|ControlMask, XK_Return, spawn, {.v = TermCmd2 } },
  { MODKEY|ShiftMask, XK_f, spawn, {.v = Browser1 } },
  { MODKEY|ControlMask, XK_f, spawn, {.v = Browser2 } },
  { MODKEY|ShiftMask, XK_m, spawn, {.v = MailClient1 } },
  { MODKEY|ShiftMask, XK_a, spawn, {.v = Editor1 } },
  { MODKEY|ControlMask, XK_l, spawn, {.v = Editor2 } },
  { MODKEY|ShiftMask, XK_g, spawn, {.v = Graphics1 } },
  { MODKEY|ControlMask, XK_g, spawn, {.v = Graphics2 } },
  { MODKEY|ShiftMask, XK_w, spawn, {.v = Network1 } },
  { MODKEY|ShiftMask, XK_y, spawn, {.v = Schematics3 } },
  { MODKEY|ControlMask, XK_F1, spawn, {.v = RdpWe } },
  { MODKEY|ControlMask, XK_F2, spawn, {.v = RdpPerun } },
  { MODKEY|ControlMask, XK_F3, spawn, {.v = RdpAll } },
  { MODKEY|ControlMask, XK_F4, spawn, {.v = Rdp204 } },
  { MODKEY|ControlMask, XK_F4, spawn, {.v = Rdp205 } },
  { MODKEY|ControlMask, XK_s, spawn, {.v = CheckSrv } },
  { MODKEY|ControlMask, XK_p, spawn, {.v = CheckPC } },
  { MODKEY, XK_z, spawn, {.v = MusicPrevious } } ,
  { MODKEY, XK_x, spawn, {.v = MusicPlay } },
  { MODKEY|ControlMask, XK_x, spawn, {.v = MusicStop } },
  { MODKEY|ShiftMask, XK_x, spawn, {.v = MusicTogglePause } },
  { MODKEY, XK_c, spawn, {.v = MusicNext } },
  { MODKEY|ShiftMask, XK_v, spawn, {.v = MusicPlayerRun } },
  { MODKEY, XK_F1, spawn, {.v = CommandRun } },
  { MODKEY|ShiftMask, XK_n, spawn, {.v = FileManager1 } },
  { MODKEY, XK_Up, spawn, SHCMD("mixer -S vol +3 >/dev/null") },
  { MODKEY, XK_Down, spawn, SHCMD("mixer -S vol -3 >/dev/null") },
  { MODKEY, XK_b, togglebar, {0} },
  { MODKEY, XK_j, focusstack, {.i = +1 } },
  { MODKEY, XK_k, focusstack, {.i = -1 } },
  { MODKEY, XK_i, incnmaster, {.i = +1 } },
  { MODKEY, XK_d, incnmaster, {.i = -1 } },
  { MODKEY, XK_h, setmfact, {.f = -0.05} },
  { MODKEY, XK_l, setmfact, {.f = +0.05} },
  { MODKEY, XK_Return, zoom, {0} },
  { MODKEY, XK_Tab, view, {0} },
  { MODKEY, XK_q, killclient, {0} },
  { MODKEY, XK_t, setlayout, {.v = &layouts[0]} },
  { MODKEY, XK_f, setlayout, {.v = &layouts[1]} },
  { MODKEY, XK_m, setlayout, {.v = &layouts[2]} },
  { MODKEY, XK_space, setlayout, {0} },
  { MODKEY|ShiftMask, XK_space, togglefloating, {0} },
  { MODKEY, XK_0, view, {.ui = ~0 } },
  { MODKEY|ShiftMask, XK_0, tag, {.ui = ~0 } },
  { MODKEY, XK_comma, focusmon, {.i = -1 } },
  { MODKEY, XK_period, focusmon, {.i = +1 } },
  { MODKEY|ShiftMask, XK_comma, tagmon, {.i = -1 } },
  { MODKEY|ShiftMask, XK_period, tagmon, {.i = +1 } },
  TAGKEYS( XK_1, 0)
  TAGKEYS( XK_2, 1)
  TAGKEYS( XK_3, 2)
  TAGKEYS( XK_4, 3)
  TAGKEYS( XK_5, 4)
  TAGKEYS( XK_6, 5)
  TAGKEYS( XK_7, 6)
  TAGKEYS( XK_8, 7)
  TAGKEYS( XK_9, 8)
  { MODKEY|ShiftMask, XK_Escape, reexec, {.v = (const char *[]){ "/usr/local/bin/dwm", NULL } } },
  { MODKEY|ControlMask, XK_Escape, quit, {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click, event mask, button, function, argument */
  { ClkLtSymbol, 0, Button1, setlayout, {0} },
  { ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]} },
  { ClkWinTitle, 0, Button2, zoom, {0} },
  { ClkStatusText, 0, Button2, spawn, {.v = TermCmd1 } },
  { ClkClientWin, MODKEY, Button1, movemouse, {0} },
  { ClkClientWin, MODKEY, Button2, togglefloating, {0} },
  { ClkClientWin, MODKEY, Button3, resizemouse, {0} },
  { ClkTagBar, 0, Button1, view, {0} },
  { ClkTagBar, 0, Button3, toggleview, {0} },
  { ClkTagBar, MODKEY, Button1, tag, {0} },
  { ClkTagBar, MODKEY, Button3, toggletag, {0} },
};

