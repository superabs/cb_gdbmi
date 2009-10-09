#ifndef _DEBUGGER_GDB_MI_FRAME_H_
#define _DEBUGGER_GDB_MI_FRAME_H_

#include <wx/string.h>

namespace dbg_mi
{

class ResultValue;

class Frame
{
public:
    Frame() :
        m_line(-1),
        m_address(0),
        m_has_valid_source(false)
    {
    }

    bool ParseOutput(ResultValue const &output_value);
    bool ParseFrame(ResultValue const &output_value);

    int GetLine() const { return m_line; }
    wxString const & GetFilename() const { return m_filename; }
    wxString const & GetFullFilename() const { return m_full_filename; }
    wxString const & GetFunction() const { return m_function; }
    unsigned long int GetAddress() const { return m_address; }

    bool HasValidSource() const { return m_has_valid_source; }

private:
    wxString m_filename;
    wxString m_full_filename;
    wxString m_function;
    int m_line;
    unsigned long int m_address;
    bool m_has_valid_source;
};

class FrameArguments
{
public:
    FrameArguments();
    bool Attach(ResultValue const &output);

    int GetCount() const;
    bool GetFrame(int index, wxString &args) const;
private:
    ResultValue const *m_stack_args;
};

struct StoppedReason
{
    enum Type
    {
        Unknown = 0,
        BreakpointHit, // A breakpoint was reached.
//        WatchpointTrigger, // A watchpoint was triggered.
//        ReadWatchpointTrigger, // A read watchpoint was triggered.
//        AccessWatchpointTrigger, // An access watchpoint was triggered.
//        FunctionFinished, // An -exec-finish or similar CLI command was accomplished.
//        LocationReached // An -exec-until or similar CLI command was accomplished.
//        WatchpointScope, // A watchpoint has gone out of scope.
//        EndSteppingRange, // An -exec-next, -exec-next-instruction, -exec-step, -exec-step-instruction or similar CLI command was accomplished.
//        ExitedSignalled, // The inferior exited because of a signal.
//        Exited, // The inferior exited.
        ExitedNormally, // The inferior exited normally.
//        SignalReceived // A signal was received by the inferior.
    };

    StoppedReason(Type type_) : type(type_)
    {
    }
    bool operator == (StoppedReason const &t) const
    {
        return type == t.type;
    }
    bool operator != (StoppedReason const &t) const
    {
        return !(*this == t);
    }

    static StoppedReason Parse(ResultValue const &value);
private:
    Type type;
};



} // namespace dbg_mi


#endif // _DEBUGGER_GDB_MI_FRAME_H_
