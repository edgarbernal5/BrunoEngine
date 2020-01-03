﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TrioApi.Net.Maths
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2 : IEquatable<Vector2>
    {
        public float X;
        public float Y;

        public Vector2(float _x, float _y)
        {
            X = _x;
            Y = _y;
        }

        public Vector2(double _x, double _y)
        {
            X = (float)_x;
            Y = (float)_y;
        }

        public Vector2(float sameValue)
        {
            X = sameValue;
            Y = sameValue;
        }

        /// <summary>Tests vectors for equality.</summary>
        /// <param name="value1">Source vector.</param>
        /// <param name="value2">Source vector.</param>
        public static bool operator ==(Vector2 value1, Vector2 value2)
        {
            return value1.X == value2.X && value1.Y == value2.Y;
        }

        /// <summary>Tests vectors for inequality.</summary>
        /// <param name="value1">Vector to compare.</param>
        /// <param name="value2">Vector to compare.</param>
        public static bool operator !=(Vector2 value1, Vector2 value2)
        {
            return value1.X != value2.X || value1.Y != value2.Y;
        }

        /// <summary>Determines whether the specified Object is equal to the Vector2.</summary>
		/// <param name="other">The Vector2 to compare with the current Vector2.</param>
		public bool Equals(Vector2 other)
        {
            return this.X == other.X && this.Y == other.Y;
        }

        /// <summary>Returns a value that indicates whether the current instance is equal to a specified object.</summary>
        /// <param name="obj">Object to make the comparison with.</param>
        public override bool Equals(object obj)
        {
            bool result = false;
            if (obj is Vector2)
            {
                result = this.Equals((Vector2)obj);
            }
            return result;
        }

        /// <summary>Gets the hash code of the vector object.</summary>
        public override int GetHashCode()
        {
            return this.X.GetHashCode() + this.Y.GetHashCode();
        }
    }
}