
#include <string>									
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////

class CTimePiece
{	

public:
																		// Flytter konstruktørene til public slik at klasser som arver kan bruke denne konstruktøren
	CTimePiece()														// Lager en ny konstruktør som lager en ny m_pTime når den blir kalt
	{																	
		m_pTime = new time_t;
		time(m_pTime);
	}
	CTimePiece(time_t* p)	{ m_pTime = p; } 

	void UpdateTime()
	{	
		if (!m_pTime) {
			m_pTime = new time_t;
			time(m_pTime);
		}
	}

	~CTimePiece()			{ delete m_pTime; }	
	time_t GetTime() const	{ return *m_pTime; }						 // Må spesifisere const så ingenting endres og returnere verdien (ikke pointeren)
	virtual void PrintTime() = 0;					 

	time_t *m_pTime = nullptr;											// Bør alltid initialisere member variabler 
};

/////////////////////////////////////////////////////////////////////////

class CClock : public CTimePiece
{
public:
	CClock() : CTimePiece() {}
	void PrintTime()								
	{	
		printf("\nThe current time is %s", ctime(m_pTime)); 
	}
};

/////////////////////////////////////////////////////////////////////////

class CStopWatch : public CClock
{
	const char* GetFormatStr()											// Endret slik at man slipper å måtte gå igjennom std::string 
	{	
		const char* c_str = ("\nElapsed Time is %f seconds.");
		return c_str;
	}

public:
	CStopWatch(time_t* endTime)										   // Tatt med member variable  
	{	
		m_pEndTime = endTime;
		UpdateTime(); 
	}
	~CStopWatch() { delete m_pEndTime; }
	CStopWatch& operator=(const CStopWatch& rhs)
	{	

		if (this == &rhs) {											 // Bør sjekke om venstre side (this) er lik høyre side (rhs). Bruker unødvendig minne å slette å lage ny m_pTime
			return *this;
		}
		delete m_pTime;
		m_pTime = new time_t;
		*m_pTime = rhs.GetTime();									 // Returnerer verdien til m_pTime. Bør den returnere en ny peker til samme sted i minne? 
		//*m_pEndTime = rhs.GetEndTime();
		return *this; 
	}

	void Start()	{	UpdateTime(); }
	void Stop()
	{	if (!m_pEndTime)
			m_pEndTime = new time_t;
		time(m_pEndTime); }
	
	void PrintTime() 						
	{	
		double fDiff = difftime(*m_pEndTime, GetTime()); 
		printf(GetFormatStr(), fDiff); 
	}
	//time_t GetEndTime() const { return *m_pEndTime; }

	time_t *m_pEndTime = nullptr;
};

/////////////////////////////////////////////////////////////////////////

std::vector<int> GenerateABunchOfRandomNumbers(long count)
{
	std::vector<int> vec;
	for (long n = 0; n < count; ++n)
	{
		vec.push_back(rand());
	}
	return vec;
}

/////////////////////////////////////////////////////////////////////////

void ABillionRandomNumbers()											// Gjorde om funksjonen til void slik at man kan bruke funksjonen uten å ha en CStopWatch
{																		// Tatt ut Start og Stop funksjonene 
	for (long n = 0; n < 1000; ++n)					
	{
		std::vector<int> vec = GenerateABunchOfRandomNumbers(100000); 
		printf("\n... Finished Block Number %ld of %ld random numbers", 
			n, vec.size());
	}
}

/////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	try		// Just in case something goes wrong ...
	{		
		time_t* times = new time_t;										// Lager time_t pointer 
		time(times);													// Initialiserer pointeren med time() 
		
		CStopWatch* pStopWatch = new CStopWatch(times);					// Lager CStopWatch pointer
		CClock* pClock = new CClock();									// Lager CClock pointer
		
		// Print the current time
		pClock->PrintTime();

		// Generate 1 billion random numbers
		srand((unsigned) pStopWatch->GetTime());
		printf("\nGenerating 1 billion random numbers...");
		pStopWatch->Start();											// Start funksjonen som er tatt ut fra ABillionRandomNumbers 
		ABillionRandomNumbers();										// Kaller ABillionRandomNumbers uten å måtte bruke pStopWatch
		pStopWatch->Stop();												// Stop funksjonen som er tatt ut fra ABillionRandomNumbers 
		pStopWatch->PrintTime();

		delete pStopWatch;
		delete pClock;
		delete times;													// Må slette pointere som er laget, inkl times 
	}
	catch (...)
	{
	}
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

