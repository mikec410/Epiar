/*
 * Filename      : planets.cpp
 * Author(s)     : Chris Thielen (chris@luethy.net)
 * Date Created  : Unknown (2006?)
 * Last Modified : Monday, November 16 2009
 * Purpose       : 
 * Notes         :
 */

#include "Sprites/planets.h"
#include "Utilities/log.h"
#include "Utilities/parser.h"

Planets *Planets::pInstance = 0; // initialize pointer

Planets *Planets::Instance( void ) {
	if( pInstance == 0 ) { // is this the first call?
		pInstance = new Planets; // create the sold instance
	}
	return( pInstance );
}

bool Planets::Load( string filename ) {
	Parser<cPlanet> parser;
	
	planets = parser.Parse( filename, "planets", "planet" );

	for( list<cPlanet *>::iterator i = planets.begin(); i != planets.end(); ++i ) {
		(*i)->_dbg_PrintInfo();
	}

	return true;
}

// Adds all planets in the manager (this, Planets) to the spritelist
void Planets::RegisterAll( SpriteManager *sprites ) {
	if( !sprites ) {
		Log::Warning( "Invalid spritelist passed to planets manager." );
		
		return;
	}

	for( list<cPlanet *>::iterator i = planets.begin(); i != planets.end(); ++i ) {
		sprites->Add( (*i) );
	}	
}


void Planets_Lua::RegisterPlanets(lua_State *L){
	static const luaL_Reg PlanetFunctions[] = {
		{"Name", &Planets_Lua::GetName},
		{"Position", &Planets_Lua::GetPosition},
		{"Alliance", &Planets_Lua::GetAlliance},
		{"Traffic", &Planets_Lua::GetTraffic},
		{"MilitiaSize", &Planets_Lua::GetMilitiaSize},
		{"Landable", &Planets_Lua::GetLandable},
		{NULL, NULL}
	};
	luaL_newmetatable(L, EPIAR_PLANET);
	luaL_openlib(L, EPIAR_PLANET, PlanetFunctions,0);  
}


cPlanet **Planets_Lua::pushPlanet(lua_State *luaVM){
	cPlanet **s = (cPlanet **)lua_newuserdata(luaVM, sizeof(cPlanet*));
    *s = new cPlanet();
    luaL_getmetatable(luaVM, EPIAR_PLANET);
    lua_setmetatable(luaVM, -2);
    return s;
}

cPlanet **Planets_Lua::checkPlanet(lua_State *luaVM, int index){
  cPlanet **p;
  luaL_checktype(luaVM, index, LUA_TUSERDATA);
  p = (cPlanet**)luaL_checkudata(luaVM, index, EPIAR_PLANET);
  if (p == NULL) luaL_typerror(luaVM, index, EPIAR_PLANET);
  return p;
}

int Planets_Lua::GetName(lua_State* L){
	int n = lua_gettop(L);  // Number of arguments
	if (n == 1) {
		cPlanet** planet= checkPlanet(L,1);
		lua_pushstring(L, (*planet)->GetName().c_str());
	} else {
		luaL_error(L, "Got %d arguments expected 1 (self)", n); 
	}
	return 1;
}

int Planets_Lua::GetPosition(lua_State* L){
	int n = lua_gettop(L);  // Number of arguments
	if (n == 1) {
		cPlanet** planet= checkPlanet(L,1);
		lua_pushnumber(L, (*planet)->GetWorldPosition().GetX() );
		lua_pushnumber(L, (*planet)->GetWorldPosition().GetY() );
	} else {
		luaL_error(L, "Got %d arguments expected 1 (self)", n); 
	}
	return 2;
}

int Planets_Lua::GetAlliance(lua_State* L){
	int n = lua_gettop(L);  // Number of arguments
	if (n == 1) {
		cPlanet** planet= checkPlanet(L,1);
		lua_pushstring(L, (*planet)->GetAlliance().c_str());
	} else {
		luaL_error(L, "Got %d arguments expected 1 (self)", n); 
	}
	return 1;
}

int Planets_Lua::GetTraffic(lua_State* L){
	int n = lua_gettop(L);  // Number of arguments
	if (n == 1) {
		cPlanet** planet= checkPlanet(L,1);
		lua_pushnumber(L, (*planet)->GetTraffic() );
	} else {
		luaL_error(L, "Got %d arguments expected 1 (self)", n); 
	}
	return 1;
}

int Planets_Lua::GetMilitiaSize(lua_State* L){
	int n = lua_gettop(L);  // Number of arguments
	if (n == 1) {
		cPlanet** planet= checkPlanet(L,1);
		lua_pushnumber(L, (*planet)->GetMilitiaSize() );
	} else {
		luaL_error(L, "Got %d arguments expected 1 (self)", n); 
	}
	return 1;
}

int Planets_Lua::GetLandable(lua_State* L){
	int n = lua_gettop(L);  // Number of arguments
	if (n == 1) {
		cPlanet** planet= checkPlanet(L,1);
		lua_pushboolean(L, (*planet)->GetLandable() );
	} else {
		luaL_error(L, "Got %d arguments expected 1 (self)", n); 
	}
	return 1;
}
