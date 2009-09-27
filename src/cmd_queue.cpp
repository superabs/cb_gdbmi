#include "cmd_queue.h"

/*
#include <algorithm>
#ifndef TEST_PROJECT
#   include <logmanager.h>
#   include <pipedprocess.h>
#endif

#include "cmd_result_parser.h"
#include "events.h"
*/

namespace dbg_mi
{
bool ParseGDBOutputLine(wxString const &line, CommandID &id, wxString &result_str)
{
    size_t pos = 0;
    while(pos < line.length() && wxIsdigit(line[pos]))
        ++pos;
    if(pos <= 10)
        return false;
    long action_id, cmd_id;

    wxString const &str_action = line.substr(0, pos - 10);
    str_action.ToLong(&action_id, 10);

    wxString const &str_cmd = line.substr(pos - 10, 10);
    str_cmd.ToLong(&cmd_id, 10);

    id = dbg_mi::CommandID(action_id, cmd_id);
    result_str = line.substr(pos, line.length() - pos);
    return true;
}

ActionsMap::ActionsMap() :
    m_last_id(1)
{
}

ActionsMap::~ActionsMap()
{
    for(Actions::iterator it = m_actions.begin(); it != m_actions.end(); ++it)
        delete *it;
}

void ActionsMap::Add(Action *action)
{
    action->SetID(m_last_id++);
    m_actions.push_back(action);
}

Action* ActionsMap::Find(int id)
{
    for(Actions::iterator it = m_actions.begin(); it != m_actions.end(); ++it)
    {
        if((*it)->GetID() == id)
            return *it;
    }
    return NULL;
}

Action const * ActionsMap::Find(int id) const
{
    for(Actions::const_iterator it = m_actions.begin(); it != m_actions.end(); ++it)
    {
        if((*it)->GetID() == id)
            return *it;
    }
    return NULL;
}

void ActionsMap::Run(CommandExecutor &executor)
{
    if(Empty())
        return;
    for(Actions::iterator it = m_actions.begin(); it != m_actions.end(); ++it)
    {
        Action &action = **it;

        if(!action.Started())
        {
            action.Start();
        }
        while(action.HasPendingCommands())
        {
            CommandID id;
            wxString const &command = action.PopPendingCommand(id);
            executor.ExecuteSimple(id, command);
        }

        if(action.Finished())
        {
            Actions::iterator del_it = it;
            --it;
            delete *del_it;
            m_actions.erase(del_it);
        }
    }
}
} // namespace dbg_mi
