#include <stdio.h>
#include <stdint.h>

#include <ratio>
#include <cmath>
#include <cstdint>
#include <cmath>
#include <type_traits>

template<typename _Type = double, typename _Scale = std::ratio<1>,
		typename _Adjust = long double>
class Distance {
public:
	typedef _Type type;
	typedef _Scale scale_type;
	typedef Distance this_type;

	Distance() :
			m_distance() {
	}

	template<typename _OtherType, typename _OtherScale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	Distance(const Distance<_OtherType, _OtherScale>& dist) :
			m_distance(
					adjust<std::ratio_divide<_OtherScale,_Scale>>(
							static_cast<_OtherType>(dist))) {
	}

	template<typename _OtherType,
			typename = typename std::enable_if<
					std::is_arithmetic<
							typename std::remove_reference<_OtherType>::type>::value,
					_OtherType>::type>
	Distance(_OtherType&& dist) :
			m_distance(adjust<std::ratio<1>>(dist)) {
	}

	~Distance() {
	}

	operator type() const {
		return m_distance;
	}

	this_type& operator ++() {
		m_distance += 1;
		return *this;
	}

	this_type operator ++(int) {
		auto prev = m_distance;
		m_distance += 1;
		return prev;
	}

	template<typename _OtherType,
			typename = typename std::enable_if<
					std::is_arithmetic<
							typename std::remove_reference<_OtherType>::type>::value,
					_OtherType>::type>
	this_type operator +(_OtherType&& dist) const {
		return m_distance + dist;
	}

	template<typename _OtherType, typename _OtherScale = _Scale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	this_type operator +(const Distance<_OtherType, _OtherScale>& dist) const {
		return m_distance
				+ adjust<std::ratio_divide<_OtherScale,_Scale>>(
						static_cast<_OtherType>(dist));
	}

	template<typename _OtherType,
			typename = typename std::enable_if<
					std::is_arithmetic<
							typename std::remove_reference<_OtherType>::type>::value,
					_OtherType>::type>
	this_type operator *(_OtherType&& dist) const {
		return m_distance * dist;
	}

	template<typename _OtherType, typename _OtherScale = _Scale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	this_type operator *(const Distance<_OtherType, _OtherScale>& dist) const {
		return m_distance
				* adjust<std::ratio_divide<_OtherScale,_Scale>>(
						static_cast<_OtherType>(dist));
	}

	template<typename _OtherType,
			typename = typename std::enable_if<
					std::is_arithmetic<
							typename std::remove_reference<_OtherType>::type>::value,
					_OtherType>::type>
	this_type& operator +=(_OtherType&& dist) {
		m_distance += dist;
		return *this;
	}

	template<typename _OtherType, typename _OtherScale = _Scale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	this_type& operator +=(const Distance<_OtherType, _OtherScale>& dist) {
		m_distance +=
				adjust<std::ratio_divide<_OtherScale,_Scale>>(
						static_cast<typename Distance<_OtherType, _OtherScale>::type>(dist));
		return *this;
	}

	template<typename _OtherType,
			typename = typename std::enable_if<
					std::is_arithmetic<
							typename std::remove_reference<_OtherType>::type>::value,
					_OtherType>::type>
	this_type& operator -=(_OtherType&& dist) {
		m_distance -= dist;
		return *this;
	}

	template<typename _OtherType, typename _OtherScale = _Scale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	this_type& operator -=(const Distance<_OtherType, _OtherScale>& dist) {
		m_distance -=
				adjust<std::ratio_divide<_OtherScale,_Scale>>(
						static_cast<typename Distance<_OtherType, _OtherScale>::type>(dist));
		return *this;
	}

	template<typename _OtherType,
			typename = typename std::enable_if<
					std::is_arithmetic<
							typename std::remove_reference<_OtherType>::type>::value,
					_OtherType>::type>
	this_type& operator *=(_OtherType&& dist) {
		m_distance *= dist;
		return *this;
	}

	template<typename _OtherType, typename _OtherScale = _Scale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	this_type& operator *=(const Distance<_OtherType, _OtherScale>& dist) {
		m_distance *=
				adjust<std::ratio_divide<_OtherScale,_Scale>>(
						static_cast<typename Distance<_OtherType, _OtherScale>::type>(dist));
		return *this;
	}

	template<typename _OtherType,
			typename = typename std::enable_if<
					std::is_arithmetic<
							typename std::remove_reference<_OtherType>::type>::value,
					_OtherType>::type>
	this_type& operator =(_OtherType&& dist) {
		m_distance = adjust<std::ratio<1>>(dist);
		return *this;
	}

	template<typename _OtherType, typename _OtherScale = _Scale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	bool operator <(const Distance<_OtherType, _OtherScale>& dist) {

		return m_distance <
				adjust<std::ratio_divide<_OtherScale,_Scale>>(
						static_cast<typename Distance<_OtherType, _OtherScale>::type>(dist));
	}


	template<typename _OtherType, typename _OtherScale = _Scale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	bool operator >(const Distance<_OtherType, _OtherScale>& dist) {

		return m_distance >
				adjust<std::ratio_divide<_OtherScale,_Scale>>(
						static_cast<typename Distance<_OtherType, _OtherScale>::type>(dist));
	}

	template<typename _OtherType, typename _OtherScale = _Scale,
			typename = typename std::enable_if<
					std::is_arithmetic<_OtherType>::value, _OtherType>::type>
	bool operator ==(const Distance<_OtherType, _OtherScale>& dist) {

		return m_distance ==
				adjust<std::ratio_divide<_OtherScale,_Scale>>(
						static_cast<typename Distance<_OtherType, _OtherScale>::type>(dist));
	}

private:
	template<typename _TargetScale = _Scale, typename _TargetType = type,
			typename _OtherType = type,
			typename = typename std::enable_if<
					(std::is_arithmetic<
							typename std::remove_reference<_TargetType>::type>::value
							&& std::is_arithmetic<
									typename std::remove_reference<_OtherType>::type>::value),
					_OtherType>::type>
	static _TargetType adjust(_OtherType&& dist) {
		auto is_target_integral = std::is_integral < _TargetType > ::value;
		if (_TargetScale::num == _TargetScale::den) {
			return is_target_integral ?
					static_cast<_TargetType>(round(dist)) :
					static_cast<_TargetType>(dist);
		}

		auto result = static_cast<_Adjust>(dist) * _TargetScale::num
				/ _TargetScale::den;
		return static_cast<_TargetType>(is_target_integral ? round(result) : result);

//		auto round =
//				std::is_integral < _TargetType > ::value
//						&& std::is_floating_point<
//								typename std::remove_reference<_OtherType>::type>::value;
//		if (_TargetScale::num == _TargetScale::den) {
//			return round ?
//					static_cast<_TargetType>(floor(dist + 0.5l)) :
//					static_cast<_TargetType>(dist);
//		}
//
//		auto result = static_cast<_Adjust>(dist) * _TargetScale::num
//				/ _TargetScale::den;
//		return static_cast<_TargetType>(round ? floor(result + 0.5l) : result);
	}

private:
	type m_distance;
};

template<typename _Scale = std::centi>
using DistInt = Distance < int16_t, _Scale >;
template<typename _Scale = std::ratio<1>>
using DistDec = Distance < double, _Scale >;

using DistIntCM = DistInt < std::centi >;
using DistDecM = DistDec < std::ratio<1> >;
using DistIntMM = DistInt < std::milli >;

int main() {
	DistInt< std::centi > x = 1024, y = 1024;
        int z = x * y;
        int z2 = x * y * sizeof(char);
	printf( "%d %d\n", z, z2 );

	return 0;
}
