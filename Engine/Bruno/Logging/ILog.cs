using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Bruno.Logging
{
    //ConfigSectionHandler

    public interface ILog
    {
        void Debug(string format, params object[] args);
        void Info(string format, params object[] args);

        void Warn(string format, params object[] args);

        void Error(Exception exception);
    }
}
