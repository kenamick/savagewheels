/*
    Copyright (c) 2003-2010 KenamicK Entertainment

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#include "Main.h"


CBindings::CBindings(void)
: _bLoadSuccess( false ),
  _xmlDoc( NULL )
{
}

CBindings::~CBindings(void)
{
#if _DEBUG
	if ( _xmlDoc )
		DBG( "DBG: Bindings document pointer was *not* released ! ");
#endif
}

bool CBindings::Load( const String strFilepath )
{
	LOG( "Loading bindings..." );

	_xmlDoc = new TiXmlDocument( strFilepath.c_str() );

	// keyboard
	_vKeyNames[ "SDLK_BACKSPACE" ]		= SDLK_BACKSPACE;
	_vKeyNames[ "SDLK_TAB" ]			= SDLK_TAB;
	_vKeyNames[ "SDLK_RETURN" ]			= SDLK_RETURN;
	_vKeyNames[ "SDLK_SPACE" ]			= SDLK_SPACE;
	_vKeyNames[ "SDLK_0" ]				= SDLK_0;
	_vKeyNames[ "SDLK_1" ]				= SDLK_1;
	_vKeyNames[ "SDLK_2" ]				= SDLK_2;
	_vKeyNames[ "SDLK_3" ]				= SDLK_3;
	_vKeyNames[ "SDLK_4" ]				= SDLK_4;
	_vKeyNames[ "SDLK_5" ]				= SDLK_5;
	_vKeyNames[ "SDLK_6" ]				= SDLK_6;
	_vKeyNames[ "SDLK_7" ]				= SDLK_7;
	_vKeyNames[ "SDLK_8" ]				= SDLK_8;
	_vKeyNames[ "SDLK_9" ]				= SDLK_9;
	_vKeyNames[ "SDLK_COLON" ]			= SDLK_COLON;
	_vKeyNames[ "SDLK_SEMICOLON" ]		= SDLK_SEMICOLON;
	_vKeyNames[ "SDLK_LESS" ]			= SDLK_LESS;
	_vKeyNames[ "SDLK_GREATER" ]		= SDLK_GREATER;
	_vKeyNames[ "SDLK_LEFTBRACKET" ]	= SDLK_LEFTBRACKET;
	_vKeyNames[ "SDLK_RIGHTBRACKET" ]	= SDLK_RIGHTBRACKET;
	_vKeyNames[ "SDLK_a" ]				= SDLK_a;
	_vKeyNames[ "SDLK_b" ]				= SDLK_b;
	_vKeyNames[ "SDLK_c" ]				= SDLK_c;
	_vKeyNames[ "SDLK_d" ]				= SDLK_d;
	_vKeyNames[ "SDLK_e" ]				= SDLK_e;
	_vKeyNames[ "SDLK_f" ]				= SDLK_f;
	_vKeyNames[ "SDLK_h" ]				= SDLK_h;
	_vKeyNames[ "SDLK_i" ]				= SDLK_i;
	_vKeyNames[ "SDLK_j" ]				= SDLK_j;
	_vKeyNames[ "SDLK_k" ]				= SDLK_k;
	_vKeyNames[ "SDLK_l" ]				= SDLK_l;
	_vKeyNames[ "SDLK_m" ]				= SDLK_m;
	_vKeyNames[ "SDLK_n" ]				= SDLK_n;
	_vKeyNames[ "SDLK_o" ]				= SDLK_o;
	_vKeyNames[ "SDLK_p" ]				= SDLK_p;
	_vKeyNames[ "SDLK_q" ]				= SDLK_q;
	_vKeyNames[ "SDLK_r" ]				= SDLK_r;
	_vKeyNames[ "SDLK_s" ]				= SDLK_s;
	_vKeyNames[ "SDLK_t" ]				= SDLK_t;
	_vKeyNames[ "SDLK_u" ]				= SDLK_u;
	_vKeyNames[ "SDLK_v" ]				= SDLK_v;
	_vKeyNames[ "SDLK_w" ]				= SDLK_w;
	_vKeyNames[ "SDLK_x" ]				= SDLK_x;
	_vKeyNames[ "SDLK_y" ]				= SDLK_y;
	_vKeyNames[ "SDLK_z" ]				= SDLK_z;
	_vKeyNames[ "SDLK_DELETE" ]			= SDLK_DELETE;
	_vKeyNames[ "SDLK_KP0" ]			= SDLK_KP0;
	_vKeyNames[ "SDLK_KP1" ]			= SDLK_KP1;
	_vKeyNames[ "SDLK_KP2" ]			= SDLK_KP2;
	_vKeyNames[ "SDLK_KP3" ]			= SDLK_KP3;
	_vKeyNames[ "SDLK_KP4" ]			= SDLK_KP4;
	_vKeyNames[ "SDLK_KP5" ]			= SDLK_KP5;
	_vKeyNames[ "SDLK_KP6" ]			= SDLK_KP6;
	_vKeyNames[ "SDLK_KP7" ]			= SDLK_KP7;
	_vKeyNames[ "SDLK_KP8" ]			= SDLK_KP8;
	_vKeyNames[ "SDLK_KP9" ]			= SDLK_KP9;
	_vKeyNames[ "SDLK_KP_PERIOD" ]		= SDLK_KP_PERIOD;
	_vKeyNames[ "SDLK_KP_DIVIDE" ]		= SDLK_KP_DIVIDE;
	_vKeyNames[ "SDLK_KP_MULTIPLY" ]	= SDLK_KP_MULTIPLY;
	_vKeyNames[ "SDLK_KP_PLUS" ]		= SDLK_KP_PLUS;
	_vKeyNames[ "SDLK_KP_ENTER" ]		= SDLK_KP_ENTER;
	_vKeyNames[ "SDLK_UP" ]				= SDLK_UP;
	_vKeyNames[ "SDLK_DOWN" ]			= SDLK_DOWN;
	_vKeyNames[ "SDLK_RIGHT" ]			= SDLK_RIGHT;
	_vKeyNames[ "SDLK_LEFT" ]			= SDLK_LEFT;
	_vKeyNames[ "SDLK_INSERT" ]			= SDLK_INSERT;
	_vKeyNames[ "SDLK_HOME" ]			= SDLK_HOME;
	_vKeyNames[ "SDLK_END" ]			= SDLK_END;
	_vKeyNames[ "SDLK_PAGEUP" ]			= SDLK_PAGEUP;
	_vKeyNames[ "SDLK_PAGEDOWN" ]		= SDLK_PAGEDOWN;
	_vKeyNames[ "SDLK_RSHIFT" ]			= SDLK_RSHIFT;
	_vKeyNames[ "SDLK_LSHIFT" ]			= SDLK_LSHIFT;
	_vKeyNames[ "SDLK_RCTRL" ]			= SDLK_RCTRL;
	_vKeyNames[ "SDLK_LCTRL" ]			= SDLK_LCTRL;
	_vKeyNames[ "SDLK_RALT" ]			= SDLK_RALT;
	_vKeyNames[ "SDLK_LALT" ]			= SDLK_LALT;
	// joystick
	//_vKeyNames[ "SDLK_LALT" ]			= SDLK_LALT;

	// default player keys
	_DefaultKeys[BIND_PLAYER1][BK_LEFT]	= SDLK_LEFT;
	_DefaultKeys[BIND_PLAYER1][BK_RIGHT]	= SDLK_RIGHT;
	_DefaultKeys[BIND_PLAYER1][BK_ACC]	= SDLK_UP;
	_DefaultKeys[BIND_PLAYER1][BK_BREAK]	= SDLK_DOWN;
	_DefaultKeys[BIND_PLAYER1][BK_MINE]	= SDLK_RCTRL;
	_DefaultKeys[BIND_PLAYER1][BK_HONK]	= SDLK_RALT;
	_DefaultKeys[BIND_PLAYER1][BK_BLOWUP]	= SDLK_DELETE;

	_DefaultKeys[BIND_PLAYER2][BK_LEFT]	= SDLK_d;
	_DefaultKeys[BIND_PLAYER2][BK_RIGHT]	= SDLK_g;
	_DefaultKeys[BIND_PLAYER2][BK_ACC]	= SDLK_r;
	_DefaultKeys[BIND_PLAYER2][BK_BREAK]	= SDLK_f;
	_DefaultKeys[BIND_PLAYER2][BK_MINE]	= SDLK_LCTRL;
	_DefaultKeys[BIND_PLAYER2][BK_HONK]	= SDLK_LALT;
	_DefaultKeys[BIND_PLAYER2][BK_BLOWUP]	= SDLK_TAB;

	_bLoadSuccess = _xmlDoc->LoadFile();

#define READ_KEY( pnode, knode, pid, pkey ) { \
	String strKeyTemp = _GetValues( pnode, knode ); \
	if ( strKeyTemp.size() ) { \
		int key_const = _FindKeyName( strKeyTemp ); \
		if ( -1 != key_const ) _DefaultKeys[pid][pkey] = key_const; } \
	}

	READ_KEY( BIND_NODE_PLAYER1, BIND_NODE_KEY_LEFT, BIND_PLAYER1, BK_LEFT );
	READ_KEY( BIND_NODE_PLAYER1, BIND_NODE_KEY_RIGHT, BIND_PLAYER1, BK_RIGHT );
	READ_KEY( BIND_NODE_PLAYER1, BIND_NODE_KEY_ACC, BIND_PLAYER1, BK_ACC );
	READ_KEY( BIND_NODE_PLAYER1, BIND_NODE_KEY_BREAK, BIND_PLAYER1, BK_BREAK );
	READ_KEY( BIND_NODE_PLAYER1, BIND_NODE_KEY_MINE, BIND_PLAYER1, BK_MINE );
	READ_KEY( BIND_NODE_PLAYER1, BIND_NODE_KEY_HONK, BIND_PLAYER1, BK_HONK );
	READ_KEY( BIND_NODE_PLAYER1, BIND_NODE_KEY_BLOWUP, BIND_PLAYER1, BK_BLOWUP );

	READ_KEY( BIND_NODE_PLAYER2, BIND_NODE_KEY_LEFT, BIND_PLAYER2, BK_LEFT );
	READ_KEY( BIND_NODE_PLAYER2, BIND_NODE_KEY_RIGHT, BIND_PLAYER2, BK_RIGHT );
	READ_KEY( BIND_NODE_PLAYER2, BIND_NODE_KEY_ACC, BIND_PLAYER2, BK_ACC );
	READ_KEY( BIND_NODE_PLAYER2, BIND_NODE_KEY_BREAK, BIND_PLAYER2, BK_BREAK );
	READ_KEY( BIND_NODE_PLAYER2, BIND_NODE_KEY_MINE, BIND_PLAYER2, BK_MINE );
	READ_KEY( BIND_NODE_PLAYER2, BIND_NODE_KEY_HONK, BIND_PLAYER2, BK_HONK );
	READ_KEY( BIND_NODE_PLAYER2, BIND_NODE_KEY_BLOWUP, BIND_PLAYER2, BK_BLOWUP );

#undef READ_KEY

#ifdef _DEBUG
	for( int i = 0; i < BIND_PLAYER_MAX_KEYS; i++ )
		LOG( "dbg: Player 1 key[" << i << "]=" << _DefaultKeys[BIND_PLAYER1][i] );
	for( int i = 0; i < BIND_PLAYER_MAX_KEYS; i++ )
		LOG( "dbg: Player 2 key[" << i << "]=" << _DefaultKeys[BIND_PLAYER2][i] );
#endif

	LOG( "Loading bindings...done" );

	return _bLoadSuccess;
}

int	CBindings::GetP1Key( CBindings::BindKeys Key )
{
	return _DefaultKeys[BIND_PLAYER1][ (int)Key ];
}

int	CBindings::GetP2Key( CBindings::BindKeys Key )
{
	return _DefaultKeys[BIND_PLAYER2][ (int)Key ];
}

void CBindings::Release()
{
	if ( _xmlDoc )
	{
		delete _xmlDoc;
		_xmlDoc = NULL;
	}
}

String	CBindings::_GetValues( const char *strSection, const char* strKey )
{
#define VALID_NODE(n, name) if ( !n ) { LOG( "Failed to read node '" << name << "' !" ); return ""; }

	TiXmlNode* pRoot = _xmlDoc->FirstChild( "bindings" );
	VALID_NODE( pRoot, "bindings" );

	TiXmlElement* pRootElement = pRoot->ToElement();
	VALID_NODE( pRootElement, "bindings(->ToElement())" );

	TiXmlElement* pElementSection = pRootElement->FirstChildElement( strSection );
	VALID_NODE( pElementSection, strSection );

	//TiXmlElement* pElementSection = pElementPlayer->FirstChildElement( strKey );
	//VALID_NODE( pElementSection, strPlayer );

	TiXmlElement* pElementKey = pElementSection->FirstChildElement( strKey );
	VALID_NODE( pElementKey, strKey );

	return pElementKey->GetText() != NULL ? pElementKey->GetText() : "";

#undef VALID_NODE
}

int CBindings::_FindKeyName( const String strKeyName )
{
	for( CBindings::udtKeyNames::iterator it = _vKeyNames.begin(); 
		it != _vKeyNames.end();
		it++ )
	{
		if ( it->first == strKeyName )
			return it->second;
	}

	return -1;
}
