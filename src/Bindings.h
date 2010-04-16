// Bindings.h

#ifndef __BINDINGS_INCLUDED__
#define __BINDINGS_INCLUDED__

#include "tinyxml.h"
#include <map>

#define BIND_PLAYER1			0
#define BIND_PLAYER2			1
#define BIND_PLAYER_MAX_KEYS	7

#define BIND_NODE_PLAYER1		"player1"
#define BIND_NODE_PLAYER2		"player2"
#define BIND_NODE_KEY_LEFT		"left"
#define BIND_NODE_KEY_RIGHT		"right"
#define BIND_NODE_KEY_ACC		"accelerate"
#define BIND_NODE_KEY_BREAK		"break"
#define BIND_NODE_KEY_MINE 		"laymine"
#define BIND_NODE_KEY_HONK		"honk"
#define BIND_NODE_KEY_BLOWUP	"self-destruct"

class CBindings
{
public:

	enum BindKeys
	{
		BK_LEFT = 0,
		BK_RIGHT,
		BK_ACC,
		BK_BREAK,
		BK_MINE,
		BK_HONK,
		BK_BLOWUP,
	};

	typedef std::map<String, int>	udtKeyNames;

	CBindings(void);
	virtual ~CBindings(void);

	bool	Load( const String strFilepath );
	void	Release();
	int		GetP1Key( CBindings::BindKeys Key );
	int		GetP2Key( CBindings::BindKeys Key );

protected:

	String	_GetValues( const char* strSection, const char* strKey );
	int		_FindKeyName( const String strKeyName );

protected:

	bool			_bLoadSuccess;
	TiXmlDocument*	_xmlDoc;
	udtKeyNames		_vKeyNames;
	int				_DefaultKeys[2][ BIND_PLAYER_MAX_KEYS ];

};


#endif