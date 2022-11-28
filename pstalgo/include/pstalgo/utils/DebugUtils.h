/*
Copyright 2019 Meta Berghauser Pont

This file is part of PST.

PST is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version. The GNU Lesser General Public License
is intended to guarantee your freedom to share and change all versions
of a program--to make sure it remains free software for all its users.

PST is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with PST. If not, see <http://www.gnu.org/licenses/>.
*/

#include <vector>
#include <cstdio>

template <class T>
void TWriteVec(const char* path, std::vector<T>& v)
{
	FILE* f = fopen(path, "wb");
	fwrite(v.data(), 1, sizeof(T) * v.size(), f);
	fclose(f);
}

template <class T>
std::vector<T> TReadVec(const char* path)
{
	FILE* f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	size_t file_size = ftell(f);
	fseek(f, 0, SEEK_SET);
	std::vector<T> v(file_size / sizeof(T));
	const size_t nread = fread(v.data(), 1, file_size, f);
	ASSERT(nread == file_size);
	fclose(f);
	return v;
}
