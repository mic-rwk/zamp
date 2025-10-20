#ifndef  COMMAND4ROTATE_HH
#define  COMMAND4ROTATE_HH

#ifndef __GNUG__
# pragma interface
# pragma implementation
#endif

#include <bits/stdc++.h>
#include "../../inc/AbstractInterp4Command.hh"

class Interp4Rotate : public AbstractInterp4Command {
    std::string objName;
    std::string axisName;
    double angleDegree;
    double angularSpeed;
public:
    Interp4Rotate() : objName(""), axisName(""), angleDegree(0), angularSpeed(0) {}
    virtual ~Interp4Rotate() = default;

    void PrintCmd() const override;

    void PrintSyntax() const override;

    void PrintParams() const override;

    const char* GetCmdName() const override;

    bool ExecCmd(AbstractScene &/*rScn*/, const char */*sMobObjName*/, AbstractComChannel &/*rComChann*/) override;
    
    bool ReadParams(std::istream &rStrm_CmdsList) override;

    static AbstractInterp4Command* CreateCmd();
};

#endif   // COMMAND4ROTATE_HH