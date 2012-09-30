/*
===========================================================================
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.

This file is part of Spearmint Source Code.

Spearmint Source Code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Spearmint Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Spearmint Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, Spearmint Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following
the terms and conditions of the GNU General Public License.  If not, please
request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
//
/*
=======================================================================

TEAM ORDERS MENU

=======================================================================
*/


#include "ui_local.h"


#define ART_FRAME		"menu/art/addbotframe"
#define ART_BACK0		"menu/art/back_0"
#define ART_BACK1		"menu/art/back_1"	

#define ID_LIST_BOTS		10
#define ID_LIST_CTF_ORDERS	11
#define ID_LIST_TEAM_ORDERS	12


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	frame;

	menulist_s		list;

	menubitmap_s	back;

	int				gametype;
	int				numBots;
	int				selectedBot;
	char			*bots[9];
	char			botNames[9][16];
} teamOrdersMenuInfo_t;

static teamOrdersMenuInfo_t	teamOrdersMenuInfo;

#define NUM_CTF_ORDERS		7
static const char *ctfOrders[] = {
	"I Am the Leader",
	"Defend the Base",
	"Follow Me",
	"Get Enemy Flag",
	"Camp Here",
	"Report",
	"I Relinquish Command",
	NULL
};
static const char *ctfMessages[] = {
	"i am the leader",
	"%s defend the base",
	"%s follow me",
	"%s get enemy flag",
	"%s camp here",
	"%s report",
	"i stop being the leader",
	NULL
};
#ifdef MISSIONPACK
static const char *ctfVoiceChats[] = {
	"startleader",
	"defend",
	"followme",
	"getflag",
	"camp",
	NULL,
	"stopleader",
	NULL
};
static const char *ctfButtons[] = {
	NULL,
	"8", // defend
	"10", // followme
	"7", // getflag
	NULL,
	NULL,
	NULL,
	NULL
};
#endif

#define NUM_TEAM_ORDERS		6
static const char *teamOrders[] = {
	"I Am the Leader",
	"Follow Me",
	"Roam",
	"Camp Here",
	"Report",
	"I Relinquish Command",
	NULL
};
static const char *teamMessages[] = {
	"i am the leader",
	"%s follow me",
	"%s roam",
	"%s camp here",
	"%s report",
	"i stop being the leader",
	NULL
};
#ifdef MISSIONPACK
static const char *teamVoiceChats[] = {
	"startleader",
	"followme",
	"patrol",
	"camp",
	NULL,
	"stopleader",
	NULL
};
static const char *teamButtons[] = {
	NULL,
	"10", // followme
	"9", // patrol
	NULL,
	NULL,
	NULL,
	NULL
};
#endif


/*
===============
UI_TeamOrdersMenu_BackEvent
===============
*/
static void UI_TeamOrdersMenu_BackEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	UI_PopMenu();
}


/*
===============
UI_TeamOrdersMenu_SetList
===============
*/
static void UI_TeamOrdersMenu_SetList( int id ) {
	switch( id ) {
	default:
	case ID_LIST_BOTS:
		teamOrdersMenuInfo.list.generic.id = id;
		teamOrdersMenuInfo.list.numitems = teamOrdersMenuInfo.numBots;
		teamOrdersMenuInfo.list.itemnames = (const char **)teamOrdersMenuInfo.bots;
		 break;

	case ID_LIST_CTF_ORDERS:
		teamOrdersMenuInfo.list.generic.id = id;
		teamOrdersMenuInfo.list.numitems = NUM_CTF_ORDERS;
		teamOrdersMenuInfo.list.itemnames = ctfOrders;
		break;

	case ID_LIST_TEAM_ORDERS:
		teamOrdersMenuInfo.list.generic.id = id;
		teamOrdersMenuInfo.list.numitems = NUM_TEAM_ORDERS;
		teamOrdersMenuInfo.list.itemnames = teamOrders;
		break;
	}

	teamOrdersMenuInfo.list.generic.bottom = teamOrdersMenuInfo.list.generic.top + teamOrdersMenuInfo.list.numitems * PROP_HEIGHT;
}


/*
=================
UI_TeamOrdersMenu_Key
=================
*/
sfxHandle_t UI_TeamOrdersMenu_Key( int key ) {
	menulist_s	*l;
	int	x;
	int	y;
	int	index;

	l = (menulist_s	*)Menu_ItemAtCursor( &teamOrdersMenuInfo.menu );
	if( l != &teamOrdersMenuInfo.list ) {
		return Menu_DefaultKey( &teamOrdersMenuInfo.menu, key );
	}

	switch( key ) {
		case K_MOUSE1:
			x = l->generic.left;
			y = l->generic.top;
			if( UI_CursorInRect( x, y, l->generic.right - x, l->generic.bottom - y ) ) {
				index = (uis.cursory - y) / PROP_HEIGHT;
				l->oldvalue = l->curvalue;
				l->curvalue = index;

				if( l->generic.callback ) {
					l->generic.callback( l, QM_ACTIVATED );
					return menu_move_sound;
				}
			}
			return menu_null_sound;

		case K_KP_UPARROW:
		case K_UPARROW:
			l->oldvalue = l->curvalue;

			if( l->curvalue == 0 ) {
				l->curvalue = l->numitems - 1;
			}
			else {
				l->curvalue--;
			}
			return menu_move_sound;

		case K_KP_DOWNARROW:
		case K_DOWNARROW:
			l->oldvalue = l->curvalue;

			if( l->curvalue == l->numitems - 1 ) {
				l->curvalue = 0;;
			}
			else {
				l->curvalue++;
			}
			return menu_move_sound;
	}

	return Menu_DefaultKey( &teamOrdersMenuInfo.menu, key );
}


/*
=================
UI_TeamOrdersMenu_ListDraw
=================
*/
static void UI_TeamOrdersMenu_ListDraw( void *self ) {
	menulist_s	*l;
	int			x;
	int			y;
	int			i;
	float		*color;
	qboolean	hasfocus;
	int			style;

	l = (menulist_s *)self;

	hasfocus = (l->generic.parent->cursor == l->generic.menuPosition);

	x =	320;//l->generic.x;
	y =	l->generic.y;
	for( i = 0; i < l->numitems; i++ ) {
		style = UI_LEFT|UI_SMALLFONT|UI_CENTER;
		if( i == l->curvalue ) {
			color = color_yellow;
			if( hasfocus ) {
				style |= UI_PULSE;
			}
		}
		else {
			color = color_orange;
		}

		UI_DrawProportionalString( x, y, l->itemnames[i], style, color );
		y += PROP_HEIGHT;
	}
}


/*
===============
UI_TeamOrdersMenu_ListEvent
===============
*/
static void UI_TeamOrdersMenu_ListEvent( void *ptr, int event ) {
	int		id;
	int		selection;
	char	message[256];
#ifdef MISSIONPACK
	const char **voiceChats;
	const char **buttons;
#endif

	if (event != QM_ACTIVATED)
		return;

	id = ((menulist_s *)ptr)->generic.id;
	selection = ((menulist_s *)ptr)->curvalue;

	if( id == ID_LIST_BOTS ) {
		teamOrdersMenuInfo.selectedBot = selection;
		if( teamOrdersMenuInfo.gametype == GT_CTF ) {
			UI_TeamOrdersMenu_SetList( ID_LIST_CTF_ORDERS );
		}
		else {
			UI_TeamOrdersMenu_SetList( ID_LIST_TEAM_ORDERS );
		}
		return;
	}

	if( id == ID_LIST_CTF_ORDERS ) {
		Com_sprintf( message, sizeof(message), ctfMessages[selection], teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.selectedBot] );
#ifdef MISSIONPACK
		voiceChats = ctfVoiceChats;
		buttons = ctfButtons;
#endif
	}
	else {
		Com_sprintf( message, sizeof(message), teamMessages[selection], teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.selectedBot] );
#ifdef MISSIONPACK
		voiceChats = teamVoiceChats;
		buttons = teamButtons;
#endif
	}

#ifdef MISSIONPACK
	if (teamOrdersMenuInfo.selectedBot == 0) // Everyone
	{
		if (voiceChats[selection] != NULL && buttons[selection] != NULL)
			trap_Cmd_ExecuteText( EXEC_APPEND, va( "cmd vsay_team %s; +button%s; wait; -button%s", voiceChats[selection], buttons[selection], buttons[selection] ));
		else if (voiceChats[selection] != NULL)
			trap_Cmd_ExecuteText( EXEC_APPEND, va( "cmd vsay_team %s", voiceChats[selection] ) );
		else
			trap_Cmd_ExecuteText( EXEC_APPEND, va( "say_team \"%s\"\n", message ) );
	}
	else
	{
		if (voiceChats[selection] != NULL && buttons[selection] != NULL)
			trap_Cmd_ExecuteText( EXEC_APPEND, va( "cmd vtell %s %s; +button%s; wait; -button%s", teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.selectedBot], voiceChats[selection], buttons[selection], buttons[selection] ) );
		else if (voiceChats[selection] != NULL)
			trap_Cmd_ExecuteText( EXEC_APPEND, va( "cmd vtell %s", voiceChats[selection] ) );
		else
			trap_Cmd_ExecuteText( EXEC_APPEND, va( "say_team \"%s\"\n", message ) );
	}
#else
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "say_team \"%s\"\n", message ) );
#endif
	UI_PopMenu();
}


/*
===============
UI_TeamOrdersMenu_BuildBotList
===============
*/
static void UI_TeamOrdersMenu_BuildBotList( void ) {
	uiClientState_t	cs;
	int		numPlayers;
	int		isBot;
	int		n;
	char	playerTeam = '3';
	char	botTeam;
	char	info[MAX_INFO_STRING];

	for( n = 0; n < 9; n++ ) {
		teamOrdersMenuInfo.bots[n] = teamOrdersMenuInfo.botNames[n];
	}

	trap_GetClientState( &cs );

	Q_strncpyz( teamOrdersMenuInfo.botNames[0], "Everyone", 16 );
	teamOrdersMenuInfo.numBots = 1;

	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	numPlayers = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
	teamOrdersMenuInfo.gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );

	for( n = 0; n < numPlayers && teamOrdersMenuInfo.numBots < 9; n++ ) {
		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );

		if( n == cs.clientNum ) {
			playerTeam = *Info_ValueForKey( info, "t" );
			continue;
		}

		isBot = atoi( Info_ValueForKey( info, "skill" ) );
		if( !isBot ) {
			continue;
		}

		botTeam = *Info_ValueForKey( info, "t" );
		if( botTeam != playerTeam ) {
			continue;
		}

		Q_strncpyz( teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.numBots], Info_ValueForKey( info, "n" ), 16 );
		Q_CleanStr( teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.numBots] );
		teamOrdersMenuInfo.numBots++;
	}
}


/*
===============
UI_TeamOrdersMenu_Init
===============
*/
static void UI_TeamOrdersMenu_Init( void ) {
	UI_TeamOrdersMenu_Cache();

	memset( &teamOrdersMenuInfo, 0, sizeof(teamOrdersMenuInfo) );
	teamOrdersMenuInfo.menu.fullscreen = qfalse;
	teamOrdersMenuInfo.menu.key = UI_TeamOrdersMenu_Key;

	UI_TeamOrdersMenu_BuildBotList();

	teamOrdersMenuInfo.banner.generic.type		= MTYPE_BTEXT;
	teamOrdersMenuInfo.banner.generic.x			= 320;
	teamOrdersMenuInfo.banner.generic.y			= 16;
	teamOrdersMenuInfo.banner.string			= "TEAM ORDERS";
	teamOrdersMenuInfo.banner.color				= color_white;
	teamOrdersMenuInfo.banner.style				= UI_CENTER;

	teamOrdersMenuInfo.frame.generic.type		= MTYPE_BITMAP;
	teamOrdersMenuInfo.frame.generic.flags		= QMF_INACTIVE;
	teamOrdersMenuInfo.frame.generic.name		= ART_FRAME;
	teamOrdersMenuInfo.frame.generic.x			= 320-233;
	teamOrdersMenuInfo.frame.generic.y			= 240-166;
	teamOrdersMenuInfo.frame.width				= 466;
	teamOrdersMenuInfo.frame.height				= 332;

	teamOrdersMenuInfo.list.generic.type		= MTYPE_SCROLLLIST;
	teamOrdersMenuInfo.list.generic.flags		= QMF_PULSEIFFOCUS;
	teamOrdersMenuInfo.list.generic.ownerdraw	= UI_TeamOrdersMenu_ListDraw;
	teamOrdersMenuInfo.list.generic.callback	= UI_TeamOrdersMenu_ListEvent;
	teamOrdersMenuInfo.list.generic.x			= 320-64;
	teamOrdersMenuInfo.list.generic.y			= 120;

	teamOrdersMenuInfo.back.generic.type		= MTYPE_BITMAP;
	teamOrdersMenuInfo.back.generic.name		= ART_BACK0;
	teamOrdersMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	teamOrdersMenuInfo.back.generic.callback	= UI_TeamOrdersMenu_BackEvent;
	teamOrdersMenuInfo.back.generic.x			= 0;
	teamOrdersMenuInfo.back.generic.y			= 480-64;
	teamOrdersMenuInfo.back.width				= 128;
	teamOrdersMenuInfo.back.height				= 64;
	teamOrdersMenuInfo.back.focuspic			= ART_BACK1;

	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.banner );
	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.frame );
	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.list );
	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.back );

	teamOrdersMenuInfo.list.generic.left = 220;
	teamOrdersMenuInfo.list.generic.top = teamOrdersMenuInfo.list.generic.y;
	teamOrdersMenuInfo.list.generic.right = 420;
	UI_TeamOrdersMenu_SetList( ID_LIST_BOTS );
}


/*
=================
UI_TeamOrdersMenu_Cache
=================
*/
void UI_TeamOrdersMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_FRAME );
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
}


/*
===============
UI_TeamOrdersMenu
===============
*/
void UI_TeamOrdersMenu( void ) {
	UI_TeamOrdersMenu_Init();
	UI_PushMenu( &teamOrdersMenuInfo.menu );
}


/*
===============
UI_TeamOrdersMenu_f
===============
*/
void UI_TeamOrdersMenu_f( void ) {
	uiClientState_t	cs;
	char	info[MAX_INFO_STRING];
	int		team;

	// make sure it's a team game
	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	teamOrdersMenuInfo.gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );
	if( teamOrdersMenuInfo.gametype < GT_TEAM ) {
		return;
	}

	// not available to spectators
	trap_GetClientState( &cs );
	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
	team = atoi( Info_ValueForKey( info, "t" ) );
	if( team == TEAM_SPECTATOR ) {
		return;
	}

	UI_TeamOrdersMenu();
}
