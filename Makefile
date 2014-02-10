all:
	g++ -Wall -O3 -std=c++11 generate_gaussian_data.cpp -o generate_gaussian_data
	g++ -Wall -O3 -std=c++11 generate_multinomial_data.cpp -o generate_multinomial_data
	g++ -Wall -O3 -std=c++11 generate_bernoulli_data.cpp -o generate_bernoulli_data
	g++ -Wall -O3 -std=c++11 nb_classify.cpp -o nb_classify 

clean:
	rm generate_gaussian_data generate_multinomial_data generate_bernoulli_data nb_classify training.data test.data

test:
	./generate_gaussian_data 5000 +1 5.3 2.0 12.8 3.1 18.2 1.0 1.2 3.4 56.2 2.3 > training.data
	./generate_gaussian_data 5000 -1 7.3 1.0 10.8 1.1 24.1 2.3 0.8 1.2 53.1 0.8 >> training.data
	./generate_gaussian_data 1000 +1 5.3 5.0 12.8 5.1 18.2 4.0 1.2 5.4 56.2 4.3 > test.data
	./generate_gaussian_data 1000 -1 7.3 4.0 10.8 4.1 24.1 5.3 0.8 4.2 53.1 2.8 >> test.data
	./nb_classify -d 1 training.data test.data
	./generate_gaussian_data 5000 +1 5.855 0.187 176.25 11.087 11.25 0.957 > training.data
	./generate_gaussian_data 5000 -1 5.418 0.312 132.50 23.629 7.50 1.291 >> training.data
	echo "-1 1:6.0 2:130.0 3:8.0" > test.data
	./nb_classify -d 1 training.data test.data
	./generate_gaussian_data 5000 +1 5.3 2.0 12.8 3.1 18.2 1.0 1.2 3.4 56.2 2.3 > training.data
	./generate_gaussian_data 5000 +2 7.3 1.0 10.8 1.1 24.1 2.3 0.8 1.2 53.1 0.8 >> training.data
	./generate_gaussian_data 5000 +3 6.4 1.3 11.8 2.1 30.1 1.3 2.0 0.7 59.1 0.8 >> training.data
	./generate_gaussian_data 1000 +1 5.3 5.0 12.8 5.1 18.2 4.0 1.2 5.4 56.2 4.3 > test.data
	./generate_gaussian_data 1000 +2 7.3 4.0 10.8 4.1 24.1 5.3 0.8 4.2 53.1 2.8 >> test.data
	./generate_gaussian_data 1000 +3 6.4 3.3 11.8 4.1 30.1 2.3 2.0 1.7 59.1 1.8 >> test.data
	./nb_classify -d 1 training.data test.data
	echo "+1 1:2 2:1 3:0 4:0 5:0 6:0" > training.data
	echo "+1 1:2 2:0 3:1 4:0 5:0 6:0" >> training.data
	echo "+1 1:1 2:0 3:0 4:1 5:0 6:0" >> training.data
	echo "-1 1:1 2:0 3:0 4:0 5:1 6:1" >> training.data
	echo "+1 1:3 2:0 3:0 4:0 5:1 6:1" > test.data
	./nb_classify -d 2 training.data test.data
	./generate_bernoulli_data 5000 +1 0.7 0.1 0.5 > training.data
	./generate_bernoulli_data 5000 -1 0.4 0.2 0.6 >> training.data
	./generate_bernoulli_data 1000 +1 0.75 0.05 0.55 > test.data
	./generate_bernoulli_data 1000 -1 0.45 0.25 0.45 >> test.data
	./nb_classify -d 3 training.data test.data
	echo "+1 1:1 2:0 3:0 4:0 5:1 6:1 7:1 8:1" > training.data
	echo "+1 1:0 2:0 3:1 4:0 5:1 6:1 7:0 8:0" >> training.data
	echo "+1 1:0 2:1 3:0 4:1 5:0 6:1 7:1 8:0" >> training.data	
	echo "+1 1:1 2:0 3:0 4:1 5:0 6:1 7:0 8:1" >> training.data	
	echo "+1 1:1 2:0 3:0 4:0 5:1 6:0 7:1 8:1" >> training.data	
	echo "+1 1:0 2:0 3:1 4:1 5:0 6:0 7:1 8:1" >> training.data
	echo "-1 1:0 2:1 3:1 4:0 5:0 6:0 7:1 8:0" >> training.data
	echo "-1 1:1 2:1 3:0 4:1 5:0 6:0 7:1 8:1" >> training.data	
	echo "-1 1:0 2:1 3:1 4:0 5:0 6:1 7:0 8:0" >> training.data	
	echo "-1 1:0 2:0 3:0 4:0 5:0 6:0 7:0 8:0" >> training.data	
	echo "-1 1:0 2:0 3:1 4:0 5:1 6:0 7:1 8:0" >> training.data
	echo "+1 1:1 2:0 3:0 4:1 5:1 6:1 7:0 8:1" > test.data	
	echo "-1 1:0 2:1 3:1 4:0 5:1 6:0 7:1 8:0" >> test.data
	./nb_classify -d 3 training.data test.data
