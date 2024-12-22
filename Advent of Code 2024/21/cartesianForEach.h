// copied from https://bimalgaudel.com.np/cartesian-for-each/

#include <vector>
#include <utility>
#include <functional>

template <typename R, typename F>
void cartesian_foreach( std::vector<R> const& rs, F&& call_back )
{
	using It = decltype( std::begin( rs[ 0 ] ) );
	using T = typename R::value_type;

	if ( rs.empty() ) return;

	std::vector<It> its, ends;
	its.reserve( rs.size() );
	ends.reserve( rs.size() );

	for ( auto&& r : rs )
	{
		its.push_back( std::begin( r ) );
		ends.push_back( std::end( r ) );
	}

	while ( its.front() != ends.front() )
	{
		std::vector<T> p;
		p.reserve( its.size() );

		for ( auto&& it : its )
			p.push_back( *it );

		// do something with the cartesian product
		std::invoke( std::forward<F>( call_back ), p );

		size_t i = its.size();
		while ( i > 0 )
		{
			--i;
			++its[ i ];
			if ( i == 0 ) break;
			if ( its[ i ] != ends[ i ] ) break;
			its[ i ] = std::begin( rs[ i ] );
		}
	}
}
