using System;

namespace BrunoFramework
{
    public interface IWindowManager
    {
        bool? ShowDialog(object viewModel, object context = null);

        void ShowWindow(object viewModel, object context = null, bool asChildWindow = true);
    }
}
