﻿using System;

namespace Bruno.ServiceLocation
{
    internal class FactoryFactory<T>
    {
        public Func<T> Create(ServiceContainer container)
        {
            return () => (T)container.GetInstance(typeof(T), null);
        }
    }
}
