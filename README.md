# core
C++ Webservice (Hodge Podge Framework) Demo (End to End)  

## Webframework (crow)
[Crow](https://github.com/ipkn/crow.git)

## JSON Parser
[RapidJSON](http://rapidjson.org)

## Database Connection
[Portable Component(POCO C++)](https://pocoproject.org)

## Database:
MySQL

### Conclusion:
crow seems promising, but showed significant performance degradation / hangs 
when native supported json type object with huge content was returned in response
However, returning the same json as string in response works fine, should be a bug.
