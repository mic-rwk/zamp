#ifndef  COMMAND4SET_HH
#define  COMMAND4SET_HH

#ifndef __GNUG__
# pragma interface
# pragma implementation
#endif

#include <bits/stdc++.h>
#include "../../inc/MobileObject.hh"
#include "../../inc/AbstractInterp4Command.hh"

class Interp4Set : public AbstractInterp4Command {
    std::string objName;
    double x{}, y{}, z{};
    double angX{}, angY{}, angZ{};
public:
    Interp4Set() : objName(""), x(0), y(0), z(0), angX(0), angY(0), angZ(0) {}
    virtual ~Interp4Set() = default;

    void PrintCmd() const override;

    void PrintSyntax() const override;

    void PrintParams() const override;

    const char* GetCmdName() const override;

    bool ExecCmd(AbstractScene &/*rScn*/, const char */*sMobObjName*/, AbstractComChannel &/*rComChann*/) override;
    
    bool ReadParams(std::istream &rStrm_CmdsList) override;

    static AbstractInterp4Command* CreateCmd();
};

#endif   // COMMAND4SET_HH