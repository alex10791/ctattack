
#g++ -g -Wall -o bin/main src/main.cpp

# the compiler: gcc for C program, define as g++ for C++
CC = gcc -std=gnu99

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -g
CLIBDIR = -L /usr/local/ssl/lib 
#-L ~/Documents/github/ctattack/openssl1_0_1f/ -L /usr/local/Cellar/cryptopp/5.6.3_1/lib/
CHEADER_DIR = -I src/include/ -I /usr/local/ssl/include/ -I openssl_aes_ref_impl/
# -I ~/Documents/github/ctattack/openssl1_0_1f/crypto -I ~/Documents/github/ctattack/openssl1_0_1f/include/
CLIBS   = -lm 
#-lcrypto
#-lcryptopp

# the build target executable:
TARGET = main





virt2phy: src/tools/virt2phy.c
	$(CC) $(CFLAGS) -o bin/virt2phy src/tools/virt2phy.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)




aes_ri_profiling: src/ssa/aes_ri_profiling.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/aes_ri_profiling src/ssa/aes_ri_profiling.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)




# Bernstein Attack

aes_simple_encrypt_ri: src/bernstein/servers/aes_simple_encrypt_ri.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/aes_simple_encrypt_ri src/bernstein/servers/aes_simple_encrypt_ri.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

aes_simple_client: src/bernstein/clients/aes_simple_client.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/aes_simple_client src/bernstein/clients/aes_simple_client.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

aes_client_v1: src/bernstein/clients/aes_client_v1.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/aes_client_v1 src/bernstein/clients/aes_client_v1.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)



bernstein_original_server: src/bernstein/original/server.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/server src/bernstein/original/server.c src/ctattack.c -lcrypto $(CHEADER_DIR)

bernstein_original_study: src/bernstein/original/study.c
	$(CC) $(CFLAGS) -o bin/study src/bernstein/original/study.c -lm

bernstein_original_ciphertext: src/bernstein/original/ciphertext.c
	$(CC) $(CFLAGS) -o bin/ciphertext src/bernstein/original/ciphertext.c

bernstein_original_correlate: src/bernstein/original/correlate.c
	$(CC) $(CFLAGS) -o bin/correlate src/bernstein/original/correlate.c -lm






# S$A Attack

sattolo: src/first_tests/sattolo.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/sattolo src/first_tests/sattolo.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

cacheprof_v1: src/ssa/cacheprof_v1.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cacheprof_v1 src/ssa/cacheprof_v1.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

cacheprof_v2: src/ssa/cacheprof_v2.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cacheprof_v2 src/ssa/cacheprof_v2.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

cacheprof_v3: src/ssa/cacheprof_v3.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cacheprof_v3 src/ssa/cacheprof_v3.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

masm_cacheprof_v4: src/ssa/cacheprof_v4.c src/ctattack.c
	##$(CC) $(CFLAGS) -o asm/cacheprof_v4 src/ssa/cacheprof_v4.c src/ctattack.c -O0 -masm=intel $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	gcc src/ssa/cacheprof_v4.c src/ctattack.c -S -O0 -masm=intel $(CHEADER_DIR)

cacheprof_v4: src/ssa/cacheprof_v4.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cacheprof_v4 src/ssa/cacheprof_v4.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
cache_timing_tests: src/first_tests/cache_timing_tests.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cache_timing_tests src/first_tests/cache_timing_tests.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
masm_cachemon_v4: src/ssa/cachemon_v4.c src/ctattack.c
	#$(CC) $(CFLAGS) -o bin/cachemon_v4 src/ssa/cachemon_v4.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	gcc src/ssa/cachemon_v4.c src/ctattack.c -S -O0 -masm=intel $(CHEADER_DIR)

sandybridge_i5_2435m_key_recov: src/ssa/sandybridge_i5_2435m_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/sandybridge_i5_2435m_key_recov src/ssa/sandybridge_i5_2435m_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

haswell_i7_4600_key_recov: src/ssa/haswell_i7_4600_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/haswell_i7_4600_key_recov src/ssa/haswell_i7_4600_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

skylake_i7_6700_key_recov: src/ssa/skylake_i7_6700_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/skylake_i7_6700_key_recov src/ssa/skylake_i7_6700_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

ivybridge_i7_3770_key_recov: src/ssa/ivybridge_i7_3770_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/ivybridge_i7_3770_key_recov src/ssa/ivybridge_i7_3770_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

nehalem_key_recov_v2: src/ssa/nehalem_key_recov_v2.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/nehalem_key_recov_v2 src/ssa/nehalem_key_recov_v2.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
nehalem_key_recov: src/ssa/nehalem_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/nehalem_key_recov src/ssa/nehalem_key_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
nehalem_first_byte_recov: src/ssa/nehalem_first_byte_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/nehalem_first_byte_recov src/ssa/nehalem_first_byte_recov.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
nehalem_first_byte_recov_v2: src/ssa/nehalem_first_byte_recov_v2.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/nehalem_first_byte_recov_v2 src/ssa/nehalem_first_byte_recov_v2.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
cachemon_v9_1: src/ssa/cachemon_v9_1.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v9_1 src/ssa/cachemon_v9_1.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

cachemon_v9: src/ssa/cachemon_v9.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v9 src/ssa/cachemon_v9.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
cachemon_v8: src/ssa/cachemon_v8.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v8 src/ssa/cachemon_v8.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
cachemon_v7: src/ssa/cachemon_v7.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v7 src/ssa/cachemon_v7.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
cachemon_v6: src/ssa/cachemon_v6.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v6 src/ssa/cachemon_v6.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
cachemon_v5: src/ssa/cachemon_v5.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v5 src/ssa/cachemon_v5.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

cachemon_v4: src/ssa/cachemon_v4.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v4 src/ssa/cachemon_v4.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)
	
cachemon_v3: src/ssa/cachemon_v3.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v3 src/ssa/cachemon_v3.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

cachemon_v2: src/ssa/cachemon_v2.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v2 src/ssa/cachemon_v2.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

cachemon_v1: src/ssa/cachemon_v1.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/cachemon_v1 src/ssa/cachemon_v1.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

detect_t_tables: src/trash/detect_t_tables.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/detect_t_tables src/trash/detect_t_tables.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

hugepage_attempt1: src/first_tests/hugepage_attempt1.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/hugepage_attempt1 src/first_tests/hugepage_attempt1.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

aes_client: src/clients/aes_client.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/aes_client src/clients/aes_client.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

aes_server: src/servers/aes_server.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/aes_server src/servers/aes_server.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)






aes_random_server_ri: src/servers/aes_random_server_ri.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/aes_random_server_ri src/servers/aes_random_server_ri.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

aes_server_ri: src/servers/aes_server_ri.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c
	$(CC) $(CFLAGS) -o bin/aes_server_ri src/servers/aes_server_ri.c src/ctattack.c openssl_aes_ref_impl/rijndael-alg-fst.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)






aes_random_server: src/servers/aes_random_server.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/aes_random_server src/servers/aes_random_server.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

aes_verbose_server: src/servers/aes_verbose_server.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/aes_verbose_server src/servers/aes_verbose_server.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

aes_example: src/first_tests/aes_example.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/aes_example src/first_tests/aes_example.c src/ctattack.c -O0 $(CHEADER_DIR) $(CLIBDIR) $(CLIBS)

sender: src/first_tests/sender.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/sender src/first_tests/sender.c src/ctattack.c -O0 $(CHEADER_DIR)

receiver: src/first_tests/receiver.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/receiver src/first_tests/receiver.c src/ctattack.c -O0 $(CHEADER_DIR)	

sender_v2: src/first_tests/sender_v2.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/sender_v2 src/first_tests/sender_v2.c src/ctattack.c -O0 $(CHEADER_DIR)	

receiver_v2: src/first_tests/receiver_v2.c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/receiver_v2 src/first_tests/receiver_v2.c src/ctattack.c -O0 $(CHEADER_DIR)	






# STUF THAT NEED TO BE REVISED

all: $(TARGET)

#$(TARGET): src/$(TARGET).c src/ctattack.c
#	$(CC) $(CFLAGS) -o bin/$(TARGET) src/$(TARGET).c src/ctattack.c -O0 $(CHEADER_DIR)

#bin/main: src/$(TARGET).cpp 	# this is not working, how to make bin/main depend on src/$(TARGET)




debug: src/$(TARGET).c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(TARGET).c src/ctattack.c -O0 -g $(CHEADER_DIR)

masm_intel: src/$(TARGET).c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(TARGET).c src/ctattack.c -O0 -masm=intel $(CHEADER_DIR)

run: bin/$(TARGET) 
	./bin/$(TARGET) $(ARGS)

clean:
	$(RM) $(TARGET)

