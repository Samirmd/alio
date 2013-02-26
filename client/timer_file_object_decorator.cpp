#include "client/timer_file_object_decorator.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>


namespace AIO
{

AIO::TimerFileObjectDecorator::AllTimerDataType  *AIO::TimerFileObjectDecorator::m_all_timers=NULL;

TimerFileObjectDecorator::TimerData*
                 TimerFileObjectDecorator::getTimer(const std::string &filename)
{
    if(!m_all_timers)
    {
        m_all_timers = new std::vector<TimerData*>();
    }

    for(unsigned int i=0; i<m_all_timers->size(); i++)
        if((*m_all_timers)[i]->getName()==filename)
            return (*m_all_timers)[i];

    TimerData *timer = new TimerData(filename);
    m_all_timers->push_back(timer);
    return timer;
}   // getTimer

// ----------------------------------------------------------------------------

void TimerFileObjectDecorator::printTable(FILE *out)
{
    if(!m_all_timers || m_all_timers->size()==0)
        return;

    // Determine longest name - size_t in order to be able to 
    size_t longest_name      = 0;
    unsigned long max_count[TIMER_COUNT]  = {0};
    unsigned long max_amount[TIMER_COUNT] = {0};
    for(unsigned int i=0; i<m_all_timers->size(); i++)
    {
        longest_name = std::max(longest_name, (*m_all_timers)[i]->getName().size());
        for(unsigned int j=0; j<TIMER_COUNT; j++)
        {
            max_count[j]  = std::max(max_count[j],  (*m_all_timers)[i]->getCount(j) );
            max_amount[j] = std::max(max_amount[j], (*m_all_timers)[i]->getAmount(j));
        }   // for j < TIMER_COUNT
    }


    std::ostringstream line_format;
    line_format << "%-" << longest_name<<"s ";   // "%10s"  or so
    fprintf(out,"\n");
    fprintf(out,line_format.str().c_str(),"");
    fprintf(out,"   ");  // additional indentation, which looks better



    const char *titles[] = {"open", "close", "read", "write", "seek", "misc"};

#define getNumDigits(n) (n>0 ? int(log(n)/log(10.0)+1) : 1)

    int count = longest_name;
    for(unsigned int i=0; i<TIMER_COUNT; i++)
    {
        int count_len  = getNumDigits(max_count[i]);
        int amount_len = getNumDigits(max_amount[i]);
        std::ostringstream f;
        line_format << "%8.3f (%"<<count_len<<"ld";
        if(i==TIMER_READ || i==TIMER_WRITE)
        {
            f<<"%-"<<12+count_len+amount_len<<"s ";
            line_format << " %"<<amount_len <<"ld) ";
            count += 12+count_len+amount_len+1;
        }
        else
        {
            f<<"%-"<<11+count_len<<"s ";
            line_format << ") ";
            count += 11+count_len+1;
        }
        //fprintf(out,"%s\n", f.str().c_str());
        fprintf(out,f.str().c_str(), titles[i]);
    }
    fprintf(out,"\n");
    for(unsigned int i=0; i<count; i++)
        fprintf(out,"-");
    fprintf(out,"\n");


    line_format <<std::endl;

    for(unsigned int i=0; i<m_all_timers->size(); i++)
    {
        const TimerData &t=*((*m_all_timers)[i]);
        fprintf(out,line_format.str().c_str(), t.getName().c_str(),
               t.getTime(TIMER_OPEN), t.getCount(TIMER_OPEN),
               t.getTime(TIMER_CLOSE),t.getCount(TIMER_CLOSE),
               t.getTime(TIMER_READ), t.getCount(TIMER_READ), t.getAmount(TIMER_READ),
               t.getTime(TIMER_WRITE),t.getCount(TIMER_WRITE),t.getAmount(TIMER_WRITE),
               t.getTime(TIMER_SEEK), t.getCount(TIMER_SEEK),
               t.getTime(TIMER_MISC), t.getCount(TIMER_MISC)
               );
        
    }
    
}   // printTable

}   // namespace AIO
