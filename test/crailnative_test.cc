#include "test/crailnative_test.hh"

#include <memory>
#include <fstream>

int main(int argc, char* argv[]) {
    std::string addr;
    int port;

    std::cout << "enter addr:";
    std::cin >> addr;

    std::cout << "enter port:";
    std::cin >> port;

    std::shared_ptr<CrailStore> crailStore;
    crailStore.reset(new CrailStore(addr, port));

    printf("<-[INFO] begin connect to crail\n");
    crailStore->Initialize();
    printf("<-[INFO] connect to crail end\n");

    while(1) {
        std::string key = "";
        std::string file_name = "";
        string str_data = "";
        
        std::cout << "enter key to download:";
        std::cin >> key;

        file_name = key;
        key = "/" + key;

        printf("[INFO] key is [%s]\n", key.c_str());

        CrailFile file = crailStore->Lookup<CrailFile>(const_cast<std::string&>(key)).get();
        if (!file.valid()) {
            fprintf(stderr, "[ERROR] lookup node failed\n");
            return -1;
        }

        printf("<-[NOTICE] [download_files] filename: [%s], object_key: [%s]\n", file_name.c_str(), key.c_str());

        unique_ptr<CrailInputstream> inputstream = file.inputstream();
        shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(kBufferSize);

        while (inputstream->Read(buf).get() > 0) {
            buf->Flip();
            str_data.append(reinterpret_cast<const char*>(buf->get_bytes()), buf->remaining());
            buf->Clear();
        } 

        inputstream->Close();

        printf("[INFO] begin write file [%s]\n", file_name.c_str());
        std::ofstream ofs(file_name);
        ofs << str_data;
        ofs.close();
        printf("[INFO] end write file [%s]\n", file_name.c_str());

        printf("[INFO] begin calc file hash\n");
        std::string command = "md5sum ";
        command = command + file_name;
        ::system(command.c_str());
        printf("[INFO] end calc file hash\n");
    }

    return 0;
}