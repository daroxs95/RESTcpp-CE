# ABC Bank contacts service

Programming exercise for C/C++ Developer candidates at bestvision, bootstrapped with open source libraries and projects like: 

- [oatpp](https://github.com/oatpp/oatpp)
- [oatpp-sqlite](https://github.com/oatpp/oatpp-sqlite)  
- [oatpp-swagger](https://github.com/oatpp/oatpp-swagger)
- [collector](https://github.com/PotatoBite/collector) (personal)
- sqlite3

## How to use

- `cmake -B build -S .`
- `cd build`
- `cmake --build .`
- `/myapp`

## Main endpoints

- Documentation `/swagger/ui`
- CRUD operations `/users`, `/addresses`, `/phones`.

## Notes

- The app was developed in windows, and currently linux build is failing because some dependencies relations. In any case, try to build more than one time and should do the trick.
- Due the short time there are no test available. 





