#include "client/timer_manager.hpp"

#include "client/timer_file_object_decorator.hpp"

#include <sstream>

namespace AIO
{

AIO::TimerManager::AllTimerDataType  *AIO::TimerManager::m_all_timer_data=NULL;

TimerData* TimerManager::getTimer(unsigned int count, 
                                  const std::string &filename,
                                  bool write_xml, bool write_table)
{
    if(!m_all_timer_data)
    {
        m_all_timer_data = new std::vector<TimerData*>();
    }

    for(unsigned int i=0; i<m_all_timer_data->size(); i++)
        if((*m_all_timer_data)[i]->getName()==filename)
            return (*m_all_timer_data)[i];

    TimerData *timer = new TimerData(count, filename, write_xml, write_table);
    m_all_timer_data->push_back(timer);
    return timer;
}   // getTimer

// ----------------------------------------------------------------------------
void TimerManager::atExit(FILE *out)
{
    if(!m_all_timer_data || m_all_timer_data->size()==0)
        return;

    writeXML(out);
    writeAsciiTable(out);
            
}   // atExit

// ----------------------------------------------------------------------------
void TimerManager::writeXML(FILE *out)
{
    bool found=false;
    for(unsigned int i=0; i<m_all_timer_data->size(); i++)
    {
        if((*m_all_timer_data)[i]->writeXML())
        {
            found = true;
            break;
        }
    }
    if(!found)
        return;



    fprintf(out, "<?xml version=\"1.0\"?>\n");
    fprintf(out, "<aio-timer-data>\n");
    for(unsigned int i=0; i<m_all_timer_data->size(); i++)
    {
        const TimerData &t=*((*m_all_timer_data)[i]);
        fprintf(out,"  <aio name=\"%s\" open-time=\"%f\" open-count=\"%ld\"\n",
                t.getName().c_str(), t.getTime(TIMER_OPEN),
                t.getCount(TIMER_OPEN));
        fprintf(out,"       close-time=\"%f\" close-count=\"%ld\"\n",
                t.getTime(TIMER_CLOSE), t.getCount(TIMER_CLOSE));
        fprintf(out,"       read-time=\"%f\" read-count=\"%ld\"", 
                t.getTime(TIMER_READ), t.getCount(TIMER_READ));
        if(t.getCount(TIMER_READ)>0)
            fprintf(out," read-sum=\"%ld\" read-min=\"%ld\" read-max=\"%ld\" read-avg=\"%f\"",
                    t.getAmount(TIMER_READ), t.getMinAmount(TIMER_READ),
                    t.getMaxAmount(TIMER_READ), t.getAmount(TIMER_READ)/float(t.getCount(TIMER_READ)) );
        fprintf(out, "\n");
        fprintf(out,"       write-time=\"%f\" write-count=\"%ld\"", 
                t.getTime(TIMER_WRITE), t.getCount(TIMER_WRITE));
        if(t.getCount(TIMER_WRITE)>0)
            fprintf(out," write-sum=\"%ld\" write-min=\"%ld\" write-max=\"%ld\" write-avg=\"%f\"",
                    t.getAmount(TIMER_WRITE), t.getMinAmount(TIMER_WRITE),
                    t.getMaxAmount(TIMER_WRITE), t.getAmount(TIMER_WRITE)/float(t.getCount(TIMER_WRITE)) );
        fprintf(out, "\n");

        fprintf(out,"       seek-time=\"%f\" seek-count=\"%ld\"\n", 
                t.getTime(TIMER_SEEK), t.getCount(TIMER_SEEK));
        fprintf(out,"       misc-time=\"%f\" misc-count=\"%ld\"/>\n", 
                t.getTime(TIMER_MISC), t.getCount(TIMER_MISC));

    }   // for i <m_all_timer_data->size()
    fprintf(out, "</aio-timer-data>\n");
}   // writeXML
    
// ----------------------------------------------------------------------------
void TimerManager::writeAsciiTable(FILE *out)
{
    bool found=false;
    for(unsigned int i=0; i<m_all_timer_data->size(); i++)
    {
        if((*m_all_timer_data)[i]->writeTable())
        {
            found = true;
            break;
        }
    }
    if(!found)
        return;

    // Determine longest name - size_t in order to be able to 
    size_t longest_name      = 0;
    unsigned long max_count[TIMER_COUNT]  = {0};
    unsigned long max_amount[TIMER_COUNT] = {0};
    for(unsigned int i=0; i<m_all_timer_data->size(); i++)
    {
        longest_name = std::max(longest_name, (*m_all_timer_data)[i]->getName().size());
        for(unsigned int j=0; j<TIMER_COUNT; j++)
        {
            max_count[j]  = std::max(max_count[j],  (*m_all_timer_data)[i]->getCount(j) );
            max_amount[j] = std::max(max_amount[j], (*m_all_timer_data)[i]->getAmount(j));
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

    for(unsigned int i=0; i<m_all_timer_data->size(); i++)
    {
        const TimerData &t=*((*m_all_timer_data)[i]);
        fprintf(out,line_format.str().c_str(), t.getName().c_str(),
               t.getTime(TIMER_OPEN), t.getCount(TIMER_OPEN),
               t.getTime(TIMER_CLOSE),t.getCount(TIMER_CLOSE),
               t.getTime(TIMER_READ), t.getCount(TIMER_READ), t.getAmount(TIMER_READ),
               t.getTime(TIMER_WRITE),t.getCount(TIMER_WRITE),t.getAmount(TIMER_WRITE),
               t.getTime(TIMER_SEEK), t.getCount(TIMER_SEEK),
               t.getTime(TIMER_MISC), t.getCount(TIMER_MISC)
               );
        
    }
    
}   // writeAsciiTable

}   // namespace AIO
