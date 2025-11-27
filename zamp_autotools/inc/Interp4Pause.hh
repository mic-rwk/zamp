#ifndef  COMMAND4PAUSE_HH
#define  COMMAND4PAUSE_HH

#ifndef __GNUG__
# pragma interface
# pragma implementation
#endif

#include <bits/stdc++.h>
#include "AbstractInterp4Command.hh"

class Interp4Pause : public AbstractInterp4Command {
    double time_ms;
public:
    Interp4Pause() : time_ms(0) {}
    virtual ~Interp4Pause() = default;

    void PrintCmd() const override;

    void PrintSyntax() const override;

    void PrintParams() const override;

    const char* GetCmdName() const override;

    bool ExecCmd(AbstractScene &/*rScn*/, const char */*sMobObjName*/, AbstractComChannel &/*rComChann*/) override;
    
    bool ReadParams(std::istream &rStrm_CmdsList) override;

    static AbstractInterp4Command* CreateCmd();
};

#endif   // COMMAND4PAUSE_HH