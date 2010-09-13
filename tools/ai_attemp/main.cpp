// main.cpp -

#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

#define ACTIONS  5

/******************** FSM *****************************************************/

char szActions[ACTIONS][32] = { "Take GOAL-Toy!", "Hit Random Enemy", 
						  "Take Anger_Toy", "Take HitPoints_Toy", "Take Speed_Toy" };


//float proActions[ACTIONS] = { 40, 12, 25, 15, 8 };
float proActions[ACTIONS] = { 5, 15, 30, 40, 10 };
/******************************************************************************/


// finite-state-machines AI
void doFSM()
{
	int index, i, j;
	int val = 0, tmp_val;

	// izberi random chislo
	while ( val == 0 || val < 10 )
		val = rand()%100;


	// nameri action-a s positivna razlika 
	for ( j = 0; j < ACTIONS; j++ )
	{
		if ( ( val - proActions[j] ) >= 0 )
		{
			index = j;
			break;
		}
	}

	// izberi nai-blizkiq do rnd-chisloto action
	for ( i = j; i < ACTIONS; i++ )
	{
		tmp_val = val - proActions[i];
		//cout << "\n" << tmp_val;
		
		if ( tmp_val >= 0 && tmp_val < val - proActions[index] )
		{
			//cout << "  " << i << " e po-malko ot " << index;
			index = i;
		}
	}

	//cout << "\n\n" << val;
	cout << "\n\n" << "index: " << index << "  " << szActions[index];
}


void main(void)
{

	// raondmize seed
	srand( (unsigned)time(NULL) );

	
	for ( int i = 0; i < 20; i++)
	  doFSM();

    int val = 0;

/*	for ( int i = 0; i < 60; i++ ) 
	{
	  val = (float)((rand()%100));
	
	while ( val == 0 || val < 10 )
		val = rand()%100;

	  cout << "   "  << val;
	  val = 0;
	}*/


	
	int kb;
	cin >> kb;


}