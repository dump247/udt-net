/*****************************************************************
 *
 * BSD LICENCE (http://www.opensource.org/licenses/bsd-license.php)
 *
 * Copyright (c) 2010, Cory Thomas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of the <ORGANIZATION> nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************/

#pragma once

namespace Udt
{
	ref class NativeIntArray : public System::Collections::Generic::IList<int>
	{
	private:
		const int* _data;
		int _length;

		void AssertIsValid()
		{
			if (_data == NULL)
				throw gcnew System::InvalidOperationException("Internal native array is no longer valid.");
		}

	internal:
		NativeIntArray(const int* data, int length)
		{
			_data = data;
			_length = length;
		}

		~NativeIntArray(void)
		{
			_data = NULL;
		}

	public:

		virtual property int default[int]
		{
			int get(int index)
			{
				AssertIsValid();

				if (index < 0 || index > _length)
					throw gcnew System::ArgumentOutOfRangeException("index", index, "Value must be greater than or equal to 0 and less than the length of the list.");

				return _data[index];
			}

			void set(int index, int value)
			{
				throw gcnew System::NotSupportedException("Collection is read-only.");
			}
		}

		virtual void Add(int item) = System::Collections::Generic::ICollection<int>::Add
		{
			throw gcnew System::NotSupportedException("Collection is read-only.");
		}

		virtual void Insert(int index, int item) = System::Collections::Generic::IList<int>::Insert
		{
			throw gcnew System::NotSupportedException("Collection is read-only.");
		}

		virtual void RemoveAt(int index) = System::Collections::Generic::IList<int>::RemoveAt
		{
			throw gcnew System::NotSupportedException("Collection is read-only.");
		}

		virtual bool Remove(int item) = System::Collections::Generic::ICollection<int>::Remove
		{
			throw gcnew System::NotSupportedException("Collection is read-only.");
		}

		virtual void Clear() = System::Collections::Generic::ICollection<int>::Clear
		{
			throw gcnew System::NotSupportedException("Collection is read-only.");
		}

		virtual bool Contains(int item)
		{
			return IndexOf(item) >= 0;
		}

		virtual int IndexOf(int item)
		{
			AssertIsValid();

			int itemIndex = -1;

			for (int index = 0; index < _length; ++index)
			{
				if (_data[index] == item)
				{
					itemIndex = index;
					break;
				}
			}

			return itemIndex;
		}

		virtual property int Count
		{
			int get(void) { return _length; }
		}

		virtual property bool IsReadOnly
		{
			bool get(void) = System::Collections::Generic::IList<int>::IsReadOnly::get
			{ return true; }
		}

		virtual void CopyTo(cli::array<int>^ array, int arrayIndex) = System::Collections::Generic::ICollection<int>::CopyTo
		{
			AssertIsValid();

			if (array == nullptr)
				throw gcnew System::ArgumentNullException("array");

			if (arrayIndex < 0)
				throw gcnew System::ArgumentOutOfRangeException("arrayIndex", arrayIndex, "Value must be greater than or equal to 0.");

			if (Count > (array->Length - arrayIndex))
				throw gcnew System::ArgumentException("Not enough space in target array.", "array");

			for (int index = 0; index < _length; ++index)
			{
				array->SetValue(_data[index], index + arrayIndex);
				arrayIndex += 1;
			}
		}

		virtual System::Collections::Generic::IEnumerator<int>^ GetEnumerator()
		{
			AssertIsValid();
			return gcnew Enumerator(this);
		}

		virtual System::Collections::IEnumerator^ EnumerableGetEnumerator() = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

	private:

		ref class Enumerator : System::Collections::Generic::IEnumerator<int>
		{
		private:
			NativeIntArray^ _a;
			int _index;

		public:
			Enumerator(NativeIntArray^ a) {_a = a; _index = -1;}
			~Enumerator(void) {}

			virtual property int Current
			{
				int get(void)
				{
					if (_index < 0 || _index > _a->_length)
						throw gcnew System::InvalidOperationException();

					_a->AssertIsValid();
					return _a->_data[_index];
				}
			}

			virtual property System::Object^ CurrentObject
			{
				System::Object^ get(void) = System::Collections::IEnumerator::Current::get
				{
					return Current;
				}
			}

			virtual bool MoveNext()
			{
				bool moved = false;

				if (_index < _a->_length)
				{
					_index += 1;
					moved = true;
				}

				return moved;
			}

			virtual void Reset()
			{
				throw gcnew System::NotSupportedException("Reset not supported.");
			}
		};
	};
}
