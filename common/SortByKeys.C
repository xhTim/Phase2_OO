/*
	Given two vectors of the same length, sort the first vector by the values of the second vector.
	For example, if the first vector is {1, 2, 3} and the second vector is {3, 2, 1}, the result will be {3, 2, 1}.

	Python:
		v1 = [1, 2, 3]
		v2 = [3, 2, 1]
		zipped = zip(v1, v2)
		sorted_zipped = sorted(zipped, key=lambda x: x[1])
		result = [x[0] for x in sorted_zipped]
		print(result)
	
	Feb. 2023 - JiaZhao Lin
*/

#ifndef SORTBYKEYS_H
#define SORTBYKEYS_H

std::vector<double> SortByKeys(const std::vector<double> &v1,	const std::vector<double> &v2)
{
	std::vector<double> result;

    // create a new vector by zipping v1 and v2
    vector<pair<double, double>> zipped(v1.size());
    transform(v1.begin(), v1.end(), v2.begin(), zipped.begin(), [](double a, double b) {
        return make_pair(a, b);
    });

	// sort the zipped vector by the second element of the pair
    std::sort(zipped.begin(), zipped.end(), 
          [](auto const &a, auto const &b) { return a.second < b.second; });

	// push the first element of the pair into the result vector
	for (auto p : zipped) {
		result.push_back(p.first);
	}

	return result;
}

void SortByKeys()
{
	std::vector<double> v1 {1, 2, 3, 4};
	std::vector<double> v2 {3, 2, 1, 4};

	std::vector<double> result = SortByKeys(v1, v2);

	for (auto i : result) {
		cout << i << " ";
	}
	cout << endl;

	return 0;
}

#endif