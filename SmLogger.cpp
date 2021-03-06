/* ------------------------------
 * SaveMesh plug-in messages
 */

#include <iostream> 
#include <fstream>
#include <iomanip>
#include <time.h>
#include <sys/timeb.h>

#include "SmLogger.h"

bool SmLogger::init(Ptr<UserInterface> ui, char* app_name)
{
    bool retc = true;

    if (retc)
    {
        m_ui = ui;
        if (!m_ui)
            retc = false;
    }

    if (retc)
    {
        // init log file
        // m_sLogFname = string() + getenv("TEMP") + "/" + app_name + ".log";
        m_sLogFname = string() + getenv("APPDATA") + "/Autodesk/Autodesk Fusion 360/API/AddIns/" + app_name + "/" + app_name + ".log";

        string msg = "Logging to file " + m_sLogFname;
        m_ui->messageBox(msg.c_str());
    }

    return retc;
}

void SmLogger::msg(const char* func, const char* str)
{
    if (*(m_sLogFname.c_str()))
    {
        ofstream log_file;
        log_file.open(m_sLogFname, ios::app);
        if (log_file.bad() && m_ui)
        {
            string msg = "Error to open the file " + m_sLogFname + "!";
            m_ui->messageBox(msg.c_str());
        }

        struct _timeb tstruct;
        _ftime(&tstruct);
        time_t ltime;
        time(&ltime);
        struct tm today;
        if (!_localtime64_s(&today, &ltime))
        {
            log_file << 1900 + today.tm_year << '-' << setfill('0') << setw(2) << today.tm_mon + 1 << '-' << setfill('0') << setw(2) << today.tm_mday << ' ';
            log_file << setfill('0') << setw(2) << today.tm_hour << ':' << setfill('0') << setw(2) << today.tm_min << ':' << setfill('0') << setw(2) << today.tm_sec << '.';
            log_file << setfill('0') << setw(3) << tstruct.millitm << ' ';
        }

        log_file << func << ": " << str << endl;
        log_file.close();
    }
}

void SmLogger::msgBox(const char* func, const char* str)
{
    msg(func, str);
    m_ui->messageBox(str);
}
