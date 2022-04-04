# ABC Bank contacts service

Programming exercise for C/C++ Developer candidates at bestvision, bootstrapped with open source libraries and projects like: 

- [oatpp](https://github.com/oatpp/oatpp)
- [oatpp-sqlite](https://github.com/oatpp/oatpp-sqlite)  
- [oatpp-swagger](https://github.com/oatpp/oatpp-swagger)
- [collector](https://github.com/PotatoBite/collector) (personal)
- sqlite3

## How to use

- `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
- `cmake --build build --config Release`
- `cmake --install build --prefix <desired_route>`
- Run `<desired_route>/bin/myapp` executable created. It starts REST API server on `localhost:8000`

Or by using [precompiled](https://github.com/daroxs95/RESTcpp-CE/releases) binaries.
## Main endpoints

- Documentation `/swagger/ui`
- CRUD operations `/users`, `/addresses`, `/phones`.

## Notes

- Due the short time there are no test available. 



## TODO

- Make port and ip available to change on app start (cli)
- Start using ssl

