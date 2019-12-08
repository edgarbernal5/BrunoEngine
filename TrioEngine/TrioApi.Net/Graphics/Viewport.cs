﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TrioApi.Net.Graphics
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Viewport
    {
        public float x;
        public float y;
        public float width;
        public float height;
        public float minDepth;
        public float maxDepth;

        public Viewport(float x, float y, float width, float height)
        {
            this.x = x;
            this.y = y;
            this.width = width;
            this.height = height;
            this.minDepth = 0.0f;
            this.maxDepth = 1.0f;
        }
    }
}