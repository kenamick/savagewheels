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


// define time-counter
#define TICKS SDL_GetTicks()


/////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Start()
//// Desc: inicializira timer
/////////////////////////////////////////////////////////////////////////////////////////////
void CTimer::Start()
{
	m_paused = false;
	m_starttime = TICKS;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Pause()
//// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////
void CTimer::Pause()
{
	m_paused = true;
	m_pausestart = TICKS;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Unpause()
//// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////
void CTimer::Unpause()
{
	m_paused = false;
	m_pausetime += TICKS - m_pausestart;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Time()
//// Desc: get current ticks
/////////////////////////////////////////////////////////////////////////////////////////////
Sint32 CTimer::Time()
{
	Uint32 cur_time = TICKS;
	Sint32 result = cur_time - m_pausetime;

	if ( m_paused ) 
	{
		result = result - (cur_time - m_pausestart);
	}

	return result - m_starttime;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Elapsed()
//// Desc: check if time has elapsed
/////////////////////////////////////////////////////////////////////////////////////////////
bool CTimer::Elapsed( Uint32 time_start, Uint32 interval )
{
	Sint32 cur_time = Time();

	return cur_time > time_start + interval;
	
	// if ( cur_time > time_start + interval )
	// {
	// 	time_start = (Uint32)cur_time;
	// 	return true;
	// }
	
	// return false;
}
