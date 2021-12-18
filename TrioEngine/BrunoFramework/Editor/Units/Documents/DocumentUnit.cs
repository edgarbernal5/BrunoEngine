﻿
using Bruno.Collections;
using Microsoft.Win32;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace BrunoFramework.Editor.Units
{
    public class DocumentUnit : EditorUnit, IDocumentService
    {
        private TreeNodeCollection<ICommandItem> m_menuNodes;

        public List<DocumentFactory> Factories { get => m_factories; }
        private List<DocumentFactory> m_factories;

        public DocumentUnit()
        {
            m_factories = new List<DocumentFactory>();
        }

        protected override void OnInitialize()
        {
            Editor.Services.RegisterInstance(typeof(IDocumentService), null, this);

            m_factories.Add(new SceneDocumentFactory(Editor));
        }

        protected override void OnStartup()
        {
            AddCommands();
            AddMenus();
        }

        private void AddCommands()
        {
            CommandItems.Add(
                new CommandItem("Scene", new DelegateCommand(() => NewProjectFile()))
                {
                    Text = "Scene",
                }
            );
            CommandItems.Add(
                new CommandItem("Open", new DelegateCommand(() => OpenProjectFile()))
                {
                    Text = "Open",
                }
            );
        }

        private void NewProjectFile()
        {

        }

        private void OpenProjectFile()
        {
            var openFileDialog = new OpenFileDialog()
            {
                Title = "Open File",
                CheckFileExists = true,
                CheckPathExists = true,
                Multiselect = false,
                RestoreDirectory = true,
                ValidateNames = true
            };

            openFileDialog.Filter = "x files (*.x)|*.x|fbx files (*.fbx)|*.fbx|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 1;

            bool? result = openFileDialog.ShowDialog();
            if (result != true)
                return;

            Open(openFileDialog.FileName);
        }

        private void AddMenus()
        {
            m_menuNodes = new TreeNodeCollection<ICommandItem>()
            {
                new TreeNode<ICommandItem>(new CommandGroup("FileGroup", "File"),
                    new TreeNode<ICommandItem>(new CommandGroup("FileNewGroup", "New"),
                    new TreeNode<ICommandItem>(CommandItems["Scene"])),
                    new TreeNode<ICommandItem>(CommandItems["Open"]))
            };

            Editor.MenuNodes.Add(m_menuNodes);
        }

        protected override void OnUninitialize()
        {
            RemoveMenus();
            RemoveCommands();
        }

        private void RemoveMenus()
        {
            Editor.MenuNodes.Remove(m_menuNodes);
            m_menuNodes.Clear();
        }

        private void RemoveCommands()
        {
            CommandItems.Clear();
        }

        protected override void OnShutdown()
        {
            Editor.Services.UnregisterHandler(typeof(IDocumentService));
        }

        public Document New(DocumentType documentType)
        {
            var selectedFactory = m_factories.FirstOrDefault(factory => factory.SupportedFileTypes.Contains(documentType));
            if (selectedFactory == default(DocumentFactory))
            {

            }

            var document = selectedFactory.Create(documentType);
            if (document != null)
            {
                document.New();
                var viewModel = document.CreateViewModel();
                Editor.ActivateItem(viewModel);
            }

            return document;
        }

        public Document Open(string filename)
        {
            var fileExtension = Path.GetExtension(filename).ToLower();
            var selectedFileType = m_factories.SelectMany(factory => factory.SupportedFileTypes)
                .FirstOrDefault(fileType => fileType.FileExtensions.Contains(fileExtension));
            var selectedFactory = selectedFileType.Factory;

            var projectFile = selectedFactory.Create(selectedFileType);

            projectFile.Load(filename);
            var viewModel = projectFile.CreateViewModel();
            Editor.ActivateItem(viewModel);

            return projectFile;
        }

        public override EditorDockableTabViewModel GetDockTabViewModel(string dockId)
        {
            return null;
        }
    }
}
