all:
	-g++ -o ./factorize intfact.cpp -lgmp -lboost_system -lflint  -I/usr/include/flint
