// CTimer.h -

/*

  - Title
	 Timer Class

  - Author
	 written by Peter Petrov
  
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
	

public:
	CTimer() { m_starttime = m_pausestart = 0; m_paused = false; };

	bool m_paused;

	void Start();
	int Time();
	void Pause();
	void Unpause();
	bool Elapsed( int &time_start, int interval );
};


#endif