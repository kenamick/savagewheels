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

/*

  - Title
	 Timer Class

  - Author
	 written by Petar Petrov
  
  - History	
     30.11.2003 (WD)

*/

#ifndef __CTIMER_INCLUDED
#define __CTIMER_INCLUDED

class CTimer
{
private:
	
	int m_starttime;
	int m_pausetime;
	int m_pausestart;
	bool m_paused;

public:
	CTimer() :
		m_starttime(0),
		m_pausetime(0),
		m_pausestart(0),
		m_paused(false)
	{};

	void Start();
	int Time();
	void Pause();
	void Unpause();
	bool Elapsed( int &time_start, int interval );
};


#endif
