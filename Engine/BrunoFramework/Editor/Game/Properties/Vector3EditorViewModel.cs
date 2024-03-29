﻿
using BrunoApi.Net.Maths;

namespace BrunoFramework.Editor.Game.Properties
{
    public class Vector3EditorViewModel : PropertiesEditorBase<Vector3>, ILabelledProperties
    {
        public float X
        {
            get { return Value.X; }
            set
            {
                Value = new Vector3(value, Value.Y, Value.Z);
                NotifyOfPropertyChange(() => X);
            }
        }

        public float Y
        {
            get { return Value.Y; }
            set
            {
                Value = new Vector3(Value.X, value, Value.Z);
                NotifyOfPropertyChange(() => Y);
            }
        }

        public float Z
        {
            get { return Value.Z; }
            set
            {
                Value = new Vector3(Value.X, Value.Y, value);
                NotifyOfPropertyChange(() => Z);
            }
        }

        public override void NotifyOfPropertyChange(string propertyName)
        {
            if (propertyName == "Value")
            {
                NotifyOfPropertyChange(() => X);
                NotifyOfPropertyChange(() => Y);
                NotifyOfPropertyChange(() => Z);
            }
            base.NotifyOfPropertyChange(propertyName);
        }
    }
}
