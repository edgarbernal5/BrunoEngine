﻿using Estero.Logging;
using Estero.ServiceLocation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EsteroFramework.Editor
{
    public class EditorViewModel : Screen, IEditorService
    {
        private static readonly ILog Logger = LogManager.GetLog();

        public event EventHandler<EventArgs> WindowActivated;

        public string ApplicationName
        {
            get => _applicationName;
            set
            {
                _applicationName = value;
                NotifyOfPropertyChange();
            }
        }
        private string _applicationName;

        public ServiceContainer Services { get; private set; }

        public EditorUnitCollection Units { get; private set; }

        private IEnumerable<EditorUnit> OrderedUnits
        {
            get
            {
                return Units.OrderByDescending(e => e.Priority);
            }
        }

        public int ExitCode
        {
            get { return _exitCode ?? (int)Editor.ExitCode.ERROR_SUCCESS; }
        }
        private int? _exitCode;

        public EditorWindow Window
        {
            get => _window;
            internal set
            {
                _window = value;
            }
        }
        private EditorWindow _window;

        public EditorViewModel(ServiceContainer serviceContainer)
        {
            Services = serviceContainer;
            Units = new EditorUnitCollection();
        }

        public void Configure()
        {
            Logger.Info("Configuring editor view model");
            DisplayName = "Estero Editor";

            Services.RegisterInstance(typeof(IEditorService), null, this);
            Services.RegisterPerRequest(typeof(IViewLocator), null, typeof(EditorViewLocator));
            Services.RegisterView(typeof(EditorViewModel), typeof(EditorWindow));

            foreach (var unit in OrderedUnits)
            {
                unit.Initialize(this);
            }
        }

        public void Startup()
        {
            Logger.Info("Starting editor view model");

            foreach (var unit in OrderedUnits)
            {
                unit.Startup();
            }
        }

        public void Shutdown()
        {
            Logger.Info("Shutting down editor view model");

            foreach (var unit in OrderedUnits.Reverse())
            {
                unit.Shutdown();
            }

            foreach (var unit in OrderedUnits.Reverse())
            {
                unit.Uninitialize();
            }
        }

        public void Exit(int exitCode = (int)Editor.ExitCode.ERROR_SUCCESS)
        {
            if (!_exitCode.HasValue)
            {
                _exitCode = exitCode;
            }
            _window.Close();
        }
    }
}
