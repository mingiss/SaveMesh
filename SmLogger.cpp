/* ------------------------------
 * SaveMesh plug-in messages
 */

#include <iostream> 
#include <sstream>
#include <fstream>

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
        stringstream log_fname;
        log_fname << getenv("TEMP") << "/" << app_name << ".log";
        m_sLogFname = log_fname.str();

        stringstream msg;
        msg << "Logging to file " << m_sLogFname;
        m_ui->messageBox(msg.str().c_str());
    }

    return retc;
}

void SmLogger::msg(const char* str)
{
    if (*(m_sLogFname.c_str()))
    {
        ofstream log_file;
        log_file.open(m_sLogFname, ios::app);
        log_file << str << endl;
        log_file.close();
    }
}
