% Tango Class$

classdef Tango < handle
    properties (SetAccess = private, Hidden = true)
        m_TangoID; % Tango ID to pass to functions
    end
    methods
        % C-tor
        function obj = Tango(varargin)
            obj.m_TangoID = TangoMex('new',varargin{:});
        end

        % D-tor
        function delete(obj)
            TangoMex('delete',obj.m_TangoID);
        end

        % Set Pos
        function setPos(obj,varargin)
            TangoMex('setPos',obj.m_TangoID,varargin{:});
        end

        % Displace
        function displace(obj,varargin)
            TangoMex('displace',obj.m_TangoID,varargin{:});
        end

        % Get Pos
        function varargout = getPos(obj,varargin)
            [varargout{1:nargout}] = TangoMex('getPos',obj.m_TangoID,varargin{:});
        end

        % Set Vel
        function setVel(obj,varargin)
            TangoMex('setVel',obj.m_TangoID,varargin{:});
        end

        % Get Vel
        function varargout = getVel(obj,varargin)
            [varargout{1:nargout}] = TangoMex('getVel',obj.m_TangoID,varargin{:});
        end

        % Get Actual Vel
        function varargout = getActualVel(obj,varargin)
            [varargout{1:nargout}] = TangoMex('getActualVel',obj.m_TangoID,varargin{:});
        end

        % Set Accel
        function setAccel(obj,varargin)
            TangoMex('setAccel',obj.m_TangoID,varargin{:});
        end

        % Get Accel
        function varargout = getAccel(obj,varargin)
            [varargout{1:nargout}] = TangoMex('getAccel',obj.m_TangoID,varargin{:});
        end

        % Set Dimensions
        function setDimensions(obj,varargin)
            TangoMex('setDimensions',obj.m_TangoID,varargin{:});
        end

        % Get Dimensions
        function [id, txt] = getDimensions(obj,varargin)
            id = TangoMex('getDimensions',obj.m_TangoID,varargin{:});
            
            dimtxt = {'microsteps','um','mm','Degree','Revolutions','cm','m','Inch','mil'};
            
            txt = dimtxt(id);
            
        end

        % Get Status
        function varargout = getStatus(obj,varargin)
            [varargout{1:nargout}] = TangoMex('getStatus',obj.m_TangoID,varargin{:});
        end

        % Set Joystick On
        function setJoystickOn(obj,varargin)
            TangoMex('startJoystick',obj.m_TangoID,varargin{:});
        end

        % Set Joystick Off
        function setJoystickOff(obj,varargin)
            TangoMex('stopJoystick',obj.m_TangoID,varargin{:});
        end

        % Set HandWheel On
        function setHandWheelOn(obj,varargin)
            TangoMex('startHandWheel',obj.m_TangoID,varargin{:});
        end

        % Set HandWheel Off
        function setHandWheelOff(obj,varargin)
            TangoMex('stopHandWheel',obj.m_TangoID,varargin{:});
        end
    end
end
