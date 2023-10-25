
#include <string>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////

class CTimePiece
{
	CTimePiece()			{ CTimePiece(NULL); }
	CTimePiece(time_t* p)	{ m_pTime = p; }
public:
	void UpdateTime()
	{	if (!m_pTime)
			m_pTime = new time_t;
		time(m_pTime); }

	~CTimePiece()			{ delete m_pTime; }
	time_t* GetTime()		{ return m_pTime; }
	virtual void PrintTime() = 0;

	time_t *m_pTime;
};

/////////////////////////////////////////////////////////////////////////

class CClock : public CTimePiece
{
public:
	void PrintTime()
	{	printf("\nThe current time is %s", ctime(m_pTime)); }
};

/////////////////////////////////////////////////////////////////////////

class CStopWatch : public CClock
{
	const char* GetFormatStr()
	{	std::string str;
		str = "\nElapsed Time is %f seconds.";
		return str.c_str(); }

public:
	CStopWatch()	{	UpdateTime(); }
	~CStopWatch()	{	delete m_pEndTime;	}

	CStopWatch& operator=(const CStopWatch& rhs)
	{	delete m_pTime;
		m_pTime = new time_t;
		*m_pTime = *(rhs.GetTime());
		return *this; 
	}

	void Start()	{	UpdateTime(); }
	void Stop()
	{	if (!m_pEndTime)
			m_pEndTime = new time_t;
		time(m_pEndTime); }
	
	void PrintTime()
	{	double fDiff = difftime(*m_pEndTime, *GetTime());
		printf(GetFormatStr(), fDiff); }

	time_t  *m_pEndTime;
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

CStopWatch& ABillionRandomNumbers(CStopWatch& stopWatch)
{
	stopWatch.Start();
	for (long n = 0; n < 1000; ++n);
	{
		std::vector<int> vec = GenerateABunchOfRandomNumbers(100000);
		printf("... Finished Block Number %ld of %ld random numbers\n", 
			n, vec.size());
	}
	stopWatch.Stop();

	return stopWatch;
}

/////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	try		// Just in case something goes wrong ...
	{
		CStopWatch* pStopWatch = new CStopWatch;
		CClock *pClock = pStopWatch;

		// Print the current time
		//
		pClock->PrintTime();

		// Generate 1 billion random numbers
		//
		CStopWatch& sw = *pStopWatch;
		srand((unsigned) *(pStopWatch->GetTime()));
		printf("\nGenerating 1 billion random numbers...");
		*pStopWatch = ABillionRandomNumbers(sw);
		pStopWatch->PrintTime();

		delete pStopWatch;
		delete pClock;
	}
	catch (...)
	{
	}
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
