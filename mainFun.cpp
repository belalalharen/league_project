int main() {



    bool close = true;

    do {
        int option = Option();
        switch (option) {
        case 1:
            LogIn();
            break;
        case 2:
            SignUp();
            break;
        case 3:
            cout << "Saving data and closing app..." << endl;

            close = false;
            break;
        }
    } while (close);

    return 0;

}
