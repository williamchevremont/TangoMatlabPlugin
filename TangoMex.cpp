#include "TangoLSX_API.h"
#include "mex.h"
#include "matrix.h"
#include <stdio.h>
#include <string.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // Get the command string
    char cmd[64];
        if (nrhs < 1 || mxGetString(prhs[0], cmd, sizeof(cmd)))
                mexErrMsgTxt("First input should be a command string less than 64 characters long.");
    
    int ret; // std return code from functions
    // New
    if (!strcmp("new", cmd))
    {
        // Check parameters
        if (nlhs != 1)
            mexErrMsgTxt("New: One output expected.");

        if (nrhs != 3)
            mexErrMsgTxt("New: 2 inputs required: COM-port and speed");

        // Create new Id
        int TangoId;

        if(ret = LSX_CreateLSID(&TangoId))
        {
            char err[100];
            sprintf(err,"New: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        // Assign id to return
        plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL); // create matrix
        double *pr = (double*)mxGetPr(plhs[0]); // get data adress
        *pr = TangoId; // set data at adres pointed by pr

        // Get COM-port
        char com[10];
        mxGetString(prhs[1],com,sizeof(com));

        // Get speed
        int speed;
        speed = (int)(*(mxGetPr(prhs[2])));

        // Connection
        if(ret = LSX_ConnectSimple(TangoId,1,com,speed,true))
        {
            char err[100];
            sprintf(err,"New:connect: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }
        
        if(ret = LSX_ClearPos(TangoId,7))
        {
            char err[100];
            sprintf(err,"New:ClearPos: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // Check there is a second input, should be the TangoID created by new
    if (nrhs < 2)
        mexErrMsgTxt("Second input should be the Tango Id");

    int ID = (int)(*(mxGetPr(prhs[1])));
    char wrnout[10];
    
    sprintf(wrnout,"ID: %i",ID);
    mexWarnMsgTxt(wrnout);

    // Delete
    if (!strcmp("delete", cmd))
    {
        if(ret = LSX_Disconnect(ID))
        {
            char err[100];
            sprintf(err,"Delete: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        if(ret = LSX_FreeLSID(ID))
        {
            char err[100];
            sprintf(err,"Delete: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // SetPos
    if (!strcmp("setPos", cmd))
    {
        if(nrhs < 3)
        {
            mexErrMsgTxt("setPos: Third input should be vector with [X,Y,Z,A] positions in current unit");
        }

        if(4 != mxGetM(prhs[2]) * mxGetN(prhs[2]))
        {
            mexErrMsgTxt("setPos: Input vector must have 4 elements: [X,Y,Z,A] positions in current unit");
        }

        double* pr = mxGetPr(prhs[2]);

        if(ret = LSX_MoveAbs(ID,pr[0],pr[1],pr[2],pr[3],true))
        {
            char err[100];
            sprintf(err,"setPos: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // Displace
    if (!strcmp("displace", cmd))
    {
        if(nrhs < 3)
        {
            mexErrMsgTxt("displace: Third input should be vector with [X,Y,Z,A] positions increment in current unit");
        }

        if(4 != mxGetM(prhs[2]) * mxGetN(prhs[2]))
        {
            mexErrMsgTxt("displace: Input vector must have 4 elements: [X,Y,Z,A] positions increment in current unit");
        }

        double* pr = mxGetPr(prhs[2]);

        if(ret = LSX_MoveRel(ID,pr[0],pr[1],pr[2],pr[3],true))
        {
            char err[100];
            sprintf(err,"displace: Error Tango #%i",ret);
            mexErrMsgTxt(err);
           // mexErrMsgTxt("displace: TangoMex Error");
        }

        return;
    }

    // GetPos
    if (!strcmp("getPos", cmd))
    {
        double X,Y,Z,A;

        if(ret = LSX_GetPos(ID,&X,&Y,&Z,&A))
        {
            char err[100];
            sprintf(err,"getPos: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        plhs[0] = mxCreateDoubleMatrix(1,4,mxREAL);
        double *pr = mxGetPr(plhs[0]);

        pr[0] = X;
        pr[1] = Y;
        pr[2] = Z;
        pr[3] = A;

        return;
    }

    // SetDimension
    if (!strcmp("setDimensions", cmd))
    {
        if(nrhs < 3)
        {
            mexErrMsgTxt("setDimensions: Third input should be vector with dimension ID of [X, Y, Z, A]");
        }

        if(4 != mxGetM(prhs[2]) * mxGetN(prhs[2]))
        {
            mexErrMsgTxt("setDimensions: Input vector must have 4 elements with dimension ID of [X, Y, Z, A]");
        }

        double* pr = mxGetPr(prhs[2]);

        for(int i(0);i<4;i++)
        {
            if(pr[i] < 0 || pr[i] > 8)
                mexErrMsgTxt("setDimensions: dimensions ID have to be between 0 and 8");
        }

        if(ret = LSX_SetDimensions(ID,pr[0],pr[1],pr[2],pr[3]))
        {
            char err[100];
            sprintf(err,"setDimensions: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // GetDimensions
    if (!strcmp("getDimensions", cmd))
    {
        int X,Y,Z,A;

        if(ret = LSX_GetDimensions(ID,&X,&Y,&Z,&A))
        {
            char err[100];
            sprintf(err,"getDimensions: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        plhs[0] = mxCreateDoubleMatrix(1,4,mxREAL);
        double* pr = mxGetPr(plhs[0]);

        pr[0] = X;
        pr[1] = Y;
        pr[2] = Z;
        pr[3] = A;

        return;
    }

    // SetVel
    if (!strcmp("setVel", cmd))
    {
        if(nrhs < 3)
        {
            mexErrMsgTxt("setVel: Third input should be vector with [X,Y,Z,A] speed in m/s");
        }

        if(4 != mxGetM(prhs[2]) * mxGetN(prhs[2]))
        {
            mexErrMsgTxt("setVel: Input vector must have 4 elements: [X,Y,Z,A] speed in m/s");
        }

        double* pr = mxGetPr(prhs[2]);

        if(ret = LSX_SetVel(ID,pr[0],pr[1],pr[2],pr[3]))
        {
            char err[100];
            sprintf(err,"setVel: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // GetVel
    if (!strcmp("getVel", cmd))
    {
        double X,Y,Z,A;

        if(ret = LSX_GetVel(ID,&X,&Y,&Z,&A))
        {
            char err[100];
            sprintf(err,"getVel: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        plhs[0] = mxCreateDoubleMatrix(1,4,mxREAL);
        double* pr = mxGetPr(plhs[0]);

        pr[0] = X;
        pr[1] = Y;
        pr[2] = Z;
        pr[3] = A;

        return;
    }

    // GetActualVel
    if (!strcmp("getActualVel", cmd))
    {
        double X,Y,Z,A;

        if(ret = LSX_IsVel(ID,&X,&Y,&Z,&A))
        {
            char err[100];
            sprintf(err,"getActualVel: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        plhs[0] = mxCreateDoubleMatrix(1,4,mxREAL);
        double* pr = mxGetPr(plhs[0]);

        pr[0] = X;
        pr[1] = Y;
        pr[2] = Z;
        pr[3] = A;

        return;
    }

    // SetAccel
    if (!strcmp("setAccel", cmd))
    {
        if(nrhs < 3)
        {
            mexErrMsgTxt("setAccel: Third input should be vector with [X,Y,Z,A] accelerations in m/s^2");
        }

        if(4 != mxGetM(prhs[2]) * mxGetN(prhs[2]))
        {
            mexErrMsgTxt("setAccel: Input vector must have 4 elements: [X,Y,Z,A] accelerations in m/s^2");
        }

        double* pr = mxGetPr(prhs[2]);

        if(ret = LSX_SetAccel(ID,pr[0],pr[1],pr[2],pr[3]))
        {
            char err[100];
            sprintf(err,"setAccel: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // GetAccel
    if (!strcmp("getAccel", cmd))
    {
        double X,Y,Z,A;

        if(ret = LSX_GetAccel(ID,&X,&Y,&Z,&A))
        {
            char err[100];
            sprintf(err,"getAccel: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        plhs[0] = mxCreateDoubleMatrix(1,4,mxREAL);
        double* pr = mxGetPr(plhs[0]);

        pr[0] = X;
        pr[1] = Y;
        pr[2] = Z;
        pr[3] = A;

        return;
    }

    // GetStatus
    if (!strcmp("getStatus", cmd))
    {
        char* stat;
        int len;

        if(ret = LSX_GetStatus(ID,stat,len))
        {
            char err[100];
            sprintf(err,"getStatus: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        plhs[0] = mxCreateString(stat);

        return;
    }

    // StopJoystick
    if(!strcmp("stopJoystick",cmd))
    {
        if(ret = LSX_SetJoystickOff(ID))
        {
            char err[100];
            sprintf(err,"stopJoystick: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // StopHandWheel
    if(!strcmp("stopHandWheel",cmd))
    {
        if(ret = LSX_SetHandWheelOff(ID))
        {
            char err[100];
            sprintf(err,"stopHandWheel: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }


    // StartJoystick
    if(!strcmp("startJoystick",cmd))
    {
        if(ret = LSX_SetJoystickOn(ID,true,true))
        {
            char err[100];
            sprintf(err,"startJoystick: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // StartHandWheel
    if(!strcmp("startHandWheel",cmd))
    {
        if(ret = LSX_SetHandWheelOn(ID,true,true))
        {
            char err[100];
            sprintf(err,"startHandWheel: Error Tango #%i",ret);
            mexErrMsgTxt(err);
        }

        return;
    }

    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}

