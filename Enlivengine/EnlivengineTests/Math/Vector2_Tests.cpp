#include <Enlivengine/Math/Vector2.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Vector2")
{
	en::Vector2i vi;
	vi.x = 2;
	vi.y = 3;
	en::Vector2f vf1(vi);
	en::Vector2f vf2(vf1);
	DOCTEST_CHECK(en::Math::Equals(vf1.x, vf2.x));
	DOCTEST_CHECK(en::Math::Equals(vf1.x, 2.f));
	DOCTEST_CHECK(en::Math::Equals(vf1.y, vf2.y));
	DOCTEST_CHECK(en::Math::Equals(vf1.y, 3.f));
	DOCTEST_CHECK(en::Vector2f::Equals(vf1, vf2));
	DOCTEST_CHECK(vf1 == vf2);
	en::Vector2f vf3(4.0f);
	DOCTEST_CHECK(en::Math::Equals(vf3.x, 4.f));
	DOCTEST_CHECK(en::Math::Equals(vf3.y, 4.f));
	DOCTEST_CHECK(en::Math::Equals(vf3[0], 4.f));
	DOCTEST_CHECK(en::Math::Equals(vf3(0), 4.f));
	DOCTEST_CHECK(vf3 == en::Vector2f(4.0f, 4.0f));
	DOCTEST_CHECK(en::Vector2f::Equals(vf3, en::Vector2f(4.0f, 4.0f)));
	DOCTEST_CHECK(vf1.set(vf2) == vf2);
	DOCTEST_CHECK(vf1.set(vi) == en::Vector2f(vi));
	DOCTEST_CHECK(vf1.set(4.0f) == en::Vector2f(4.0f));
	DOCTEST_CHECK(vf1.set(2.0f, 3.0f) == en::Vector2f(2.0f, 3.0f));
	en::Vector2f v1(3.0f, 7.0f);
	en::Vector2f v2(5.0f, 11.0f);
	DOCTEST_CHECK(v1 == (+v1));
	DOCTEST_CHECK(-(v1.x) == (-v1).x);
	DOCTEST_CHECK(-(v1.y) == (-v1).y);
	DOCTEST_CHECK(v1 + v2 == en::Vector2f(8.0f, 18.0f));
	DOCTEST_CHECK(v1 - v2 == en::Vector2f(-2.0f, -4.0f));
	DOCTEST_CHECK(v1 * v2 == en::Vector2f(15.0f, 77.0f));
	DOCTEST_CHECK(v1 / v2 == en::Vector2f(3.0f / 5.0f, 7.0f / 11.0f));
	DOCTEST_CHECK(v1 + 2.0f == en::Vector2f(5.0f, 9.0f));
	DOCTEST_CHECK(v1 - 2.0f == en::Vector2f(1.0f, 5.0f));
	DOCTEST_CHECK(v1 * 2.0f == en::Vector2f(6.0f, 14.0f));
	DOCTEST_CHECK(v1 / 2.0f == en::Vector2f(1.5f, 3.5f));
	DOCTEST_CHECK(1.0f - v1 == en::Vector2f(-2.0f, -6.0f));
	DOCTEST_CHECK(1.0f / v1 == en::Vector2f(1.0f / 3.0f, 1.0f / 7.0f));
	DOCTEST_CHECK(!v1.isZero());
	DOCTEST_CHECK(en::Vector2f::zero.isZero());
	v1 += v2;
	v1 -= v2;
	DOCTEST_CHECK(v1 == en::Vector2f(3.0f, 7.0f));
	v1 *= v2;
	v1 /= v2;
	DOCTEST_CHECK(v1 == en::Vector2f(3.0f, 7.0f));
	v1 += 4.0f;
	v1 -= 4.0f;
	DOCTEST_CHECK(v1 == en::Vector2f(3.0f, 7.0f));
	v1 *= 4.0f;
	v1 /= 4.0f;
	DOCTEST_CHECK(v1 == en::Vector2f(3.0f, 7.0f));
	DOCTEST_CHECK(v1 < v2);
	DOCTEST_CHECK(v2 > v1);
	DOCTEST_CHECK(v1 <= v2);
	DOCTEST_CHECK(v2 >= v1);
	DOCTEST_CHECK(en::Math::Equals(v1.dotProduct(v2), 92.0f));
	DOCTEST_CHECK(en::Math::Equals(v2.dotProduct(v1), 92.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Vector2f::dotProduct(v1, v2), 92.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Vector2f::dotProduct(v2, v1), 92.0f));
	DOCTEST_CHECK(en::Math::Equals(en::dot(v1, v2), 92.0f));
	DOCTEST_CHECK(en::Math::Equals(en::dot(v2, v1), 92.0f));
	DOCTEST_CHECK(en::Math::Equals(v1.getSquaredLength(), 58.0f));
	DOCTEST_CHECK(en::Math::Equals(v2.getSquaredLength(), 146.0f));
	DOCTEST_CHECK(en::Math::Equals(v1.getLength(), en::Math::Sqrt(58.0f)));
	DOCTEST_CHECK(en::Math::Equals(v2.getLength(), en::Math::Sqrt(146.0f)));
	v1.setLength(3.0f);
	v2.setLength(5.0f);
	DOCTEST_CHECK(en::Math::Equals(v1.getLength(), 3.0f));
	DOCTEST_CHECK(en::Math::Equals(v2.getLength(), 5.0f, 0.000001f));
	DOCTEST_CHECK(en::Vector2f::Equals(v1, en::Vector2f(9.0f / en::Math::Sqrt(58.0f), 21.0f / en::Math::Sqrt(58.0f)), 0.000001f));
	DOCTEST_CHECK(en::Vector2f::Equals(v2, en::Vector2f(25.000f / en::Math::Sqrt(146.000f), 55.000f / en::Math::Sqrt(146.000f)), 0.000001f));
	v1.normalize();
	v2.normalize();
	DOCTEST_CHECK(en::Vector2f::Equals(v1, en::Vector2f(3.0f / en::Math::Sqrt(58.0f), 7.0f / en::Math::Sqrt(58.0f)), 0.000001f));
	DOCTEST_CHECK(en::Vector2f::Equals(v2, en::Vector2f(5.0f / en::Math::Sqrt(146.000f), 11.0f / en::Math::Sqrt(146.000f)), 0.000001f));
	v1.setPolarAngle(45.0f);
	DOCTEST_CHECK(en::Math::Equals(v1.x, en::Math::Sqrt(2.0f) * 0.5f, 0.000001f));
	DOCTEST_CHECK(en::Math::Equals(v1.y, en::Math::Sqrt(2.0f) * 0.5f, 0.000001f));
	DOCTEST_CHECK(en::Math::Equals(v1.getPolarAngle(), 45.0f));
	v1.setPolarAngle(0.0f);
	DOCTEST_CHECK(en::Math::Equals(v1.x, 1.0f, 0.000001f));
	DOCTEST_CHECK(en::Math::Equals(v1.y, 0.0f, 0.000001f));
	DOCTEST_CHECK(en::Math::Equals(v1.getPolarAngle(), 0.0f));
	v1.rotate(90.0f);
	DOCTEST_CHECK(en::Math::Equals(v1.x, 0.0f, 0.000001f));
	DOCTEST_CHECK(en::Math::Equals(v1.y, 1.0f, 0.000001f));
	DOCTEST_CHECK(en::Math::Equals(v1.getPolarAngle(), 90.0f));
	v1.set(-50.0f, 50.0f);
	v2.set(50.0f, -50.0f);
	DOCTEST_CHECK(en::Vector2f::Equals(en::Vector2f::lerp(v1, v2, 0.0f), v1));
	DOCTEST_CHECK(en::Vector2f::Equals(en::Vector2f::lerp(v1, v2, 0.25f), en::Vector2f(-25.0f, 25.0f)));
	DOCTEST_CHECK(en::Vector2f::Equals(en::Vector2f::lerp(v1, v2, 0.5f), en::Vector2f::zero));
	DOCTEST_CHECK(en::Vector2f::Equals(en::Vector2f::lerp(v1, v2, 0.75f), en::Vector2f(25.0f, -25.0f)));
	DOCTEST_CHECK(en::Vector2f::Equals(en::Vector2f::lerp(v1, v2, 1.0f), v2));
	v1.set(3.0f, 7.0f);
	v2.set(5.0f, 11.0f);
	DOCTEST_CHECK(en::Vector2f::Equals(en::Vector2f::maximum(v1, v2), v2));
	DOCTEST_CHECK(en::Vector2f::Equals(v1.maximize(v2), v2));
	DOCTEST_CHECK(en::Vector2f::Equals(en::Vector2f::minimum(en::Vector2f(3.0f, 7.0f), v2), en::Vector2f(3.0f, 7.0f)));
	DOCTEST_CHECK(en::Vector2f::Equals(v2.minimize(en::Vector2f(3.0f, 7.0f)), en::Vector2f(3.0f, 7.0f)));
}