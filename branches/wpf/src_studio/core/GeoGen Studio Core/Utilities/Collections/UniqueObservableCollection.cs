﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace GeoGen.Studio.Utilities.Collections
{
    public class UniqueObservableQueue<TValue> : IEnumerable<TValue>, INotifyCollectionChanged
    {
        protected List<TValue> values = new List<TValue>();

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public int Capacity { get; private set; }

        public IEqualityComparer<TValue> Comparer { get; private set; }

        public int Count
        {
            get
            {
                return this.values.Count;
            }
        }

        public TValue this [int index]
        {
            get
            {
                return this.values[index];
            }
        }

        /// <summary>
        /// Default capacity off the collection.
        /// </summary>
        /// <value>The default capacity.</value>
        public static int DefaultCapacity
        {
            get
            {
                return 20;
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="UniqueObservableQueue&lt;TValue&gt;"/> class with default capacity (20).
        /// </summary>
        public UniqueObservableQueue()
        {
            this.Capacity = UniqueObservableQueue<TValue>.DefaultCapacity;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="UniqueObservableQueue&lt;TValue&gt;"/> class.
        /// </summary>
        /// <param name="capacity">The capacity.</param>
        /// <param name="comparer">The comparer.</param>
        public UniqueObservableQueue(int capacity = 20, IEqualityComparer<TValue> comparer = null)
        {
            this.values.Capacity = capacity;
            this.Capacity = capacity;
            this.Comparer = comparer;
        }

        /// <summary>
        /// Adds new item to the collection. If that item already is in the collection, it will be moved to the first position. If the collection is full, the last item will be dropped.
        /// </summary>
        /// <param name="item">The item.</param>
        public virtual void Add(TValue item)
        {
            // The collection already contains the item - we will be only moving.            
            if(values.Contains(item, this.Comparer))
            {
                int oldIndex = -1;
                TValue oldItem = default(TValue);

                // Find the item in the array (cannot use IndexOf, because it doesn't support custom comparer).
                foreach (TValue value in this.values.Where(value => this.Comparer.Equals(item, value)))
                {
                    oldIndex = values.IndexOf(value);
                    oldItem = value;
                    break;
                }

                // The item is already the first -> no need to do anything.
                if (oldIndex == 0) return;

                values.RemoveAt(oldIndex);
                values.Insert(0, item);

                this.OnCollectionChanged(
                    new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, oldItem, oldIndex)
                    );

                this.OnCollectionChanged(
                    new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, item, 0)
                    );

                return;
            }

            // Crop the collection if it would be too long after the addition.
            if(this.values.Count > this.Capacity - 1)
            {
                TValue oldItem = this.values[this.Capacity - 1];

                this.values.RemoveAt(this.Capacity - 1);

                this.OnCollectionChanged(
                    new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, oldItem)
                    );
            }

            // Finally add the item.
            this.values.Insert(0, item);
            this.OnCollectionChanged(
                new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, item, 0)
                );
        }

        public virtual void Remove(TValue item)
        {
            this.values.Remove(item);
            this.OnCollectionChanged(
                new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, item)
                );
        }

        public virtual void Clear()
        {
            this.values.Clear();
            this.OnCollectionChanged(
                new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset)
                );
        }

        public virtual IEnumerator<TValue> GetEnumerator()
        {
            return new UniqueObservableQueueEnumerator(this);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        protected virtual void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
        {
            if (this.CollectionChanged != null)
            {
                this.CollectionChanged(this, e);
            }
        }

        [Serializable]
        public sealed class UniqueObservableQueueEnumerator : IEnumerator<TValue>
        {
            // Fields
            private readonly UniqueObservableQueue<TValue> collection;
            private int index;

            // Methods
            internal UniqueObservableQueueEnumerator(UniqueObservableQueue<TValue> collection)
            {
                this.collection = collection;
                this.index = -1;
            }

            public void Dispose()
            {
                this.index = -1;
            }

            public bool MoveNext()
            {
                if (this.index < this.collection.Count - 1)
                {
                    this.index++;
                    return true;
                }
                this.index = this.collection.Count;
                return false;
            }

            void IEnumerator.Reset()
            {
                this.index = -1;
            }

            // Properties
            public TValue Current
            {
                get
                {
                    if ((this.index == -1) || (this.index == this.collection.Count))
                    {
                        throw new ArgumentException("Enumerator not initialized. Call MoveNext first.");
                    }
                    return this.collection[this.index];
                }
            }

            object IEnumerator.Current
            {
                get
                {
                    if ((this.index == -1) || (this.index == this.collection.Count))
                    {
                        throw new ArgumentException("Enumerator not initialized. Call MoveNext first.");
                    }
                    return this.collection[this.index];
                }
            }
        }
    }
}
