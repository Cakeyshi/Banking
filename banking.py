import os
import sys
import PySimpleGUI as sg
from PyQt5 import QtCore, QtWidgets, QtPrintSupport
import pycountry
import ctypes
import phonenumbers
from datetime import datetime


print_content = "\t\t\t deposit: 10000\n\t\t\twithdraw: 100000"

class PyQt5PrintWindow(QtWidgets.QWidget):
    def __init__(self):
        super(PyQt5PrintWindow, self).__init__()
        self.setWindowTitle('Document Printer')
        self.editor = QtWidgets.QTextEdit(self)
        self.editor.setPlainText(print_content)
        self.editor.textChanged.connect(self.handleTextChanged)
        self.editor.setReadOnly(True)
        self.buttonPrint = QtWidgets.QPushButton('Print', self)
        self.buttonPrint.clicked.connect(self.handlePrint)
        layout = QtWidgets.QGridLayout(self)
        layout.addWidget(self.editor, 0, 0, 1, 3)
        layout.addWidget(self.buttonPrint, 1, 1)
        self.handleTextChanged()

    def handlePrint(self):
        dialog = QtPrintSupport.QPrintDialog()
        if dialog.exec_() == QtWidgets.QDialog.Accepted:
            self.editor.document().print_(dialog.printer())

    def handleTextChanged(self):
        enable = not self.editor.document().isEmpty()
        self.buttonPrint.setEnabled(enable)

def open_pyqt5_print_window():
    app = QtWidgets.QApplication(sys.argv)
    pyqt5_window = PyQt5PrintWindow()
    pyqt5_window.resize(640, 480)
    pyqt5_window.show()
    sys.exit(app.exec_())
    
    
class UserProfile(ctypes.Structure):
    _fields_ = [
        ("accountNumber", ctypes.c_char * 20),
        ("pin", ctypes.c_char * 20),
        ("firstName", ctypes.c_char * 50),
        ("middleName", ctypes.c_char * 50),
        ("lastName", ctypes.c_char * 50),
        ("birthday", ctypes.c_char * 11),
        ("age", ctypes.c_char * 10),
        ("country", ctypes.c_char * 50),
        ("phoneNumber", ctypes.c_char * 20),
        ("balance", ctypes.c_double)
    ]


#Link c and python
banking_path = os.path.join(os.path.dirname(__file__), 'banking.so')
banking = ctypes.CDLL(banking_path)


#functions
def calculate_age(birthdate):
    today = datetime.now()
    birthdate = datetime.strptime(birthdate, '%Y-%m-%d')
    age = today.year - birthdate.year - ((today.month, today.day) < (birthdate.month, birthdate.day))
    return age

def validate_phone_number(phone_number):
    try:
        parsed_number = phonenumbers.parse(phone_number)
        if phonenumbers.is_valid_number(parsed_number):
            return phonenumbers.region_code_for_number(parsed_number)
    except phonenumbers.NumberParseException:
        pass
    return None

def is_valid_pin(password):
    return all(char.isdigit() and '0' <= char <= '9' for char in password) and len(password) == 6

def is_valid_name(name):
    return all(char.isalpha() or char.isspace() for char in name) and not name.isupper()

def withdraw(account_number, withdrawal_amount):
    # Read the contents of the text file
    with open('Account.txt', 'r') as file:
        data = file.read()

    # Split the data into individual account records
    accounts = data.split('\n\n')

    # Iterate through each account record
    for account in accounts:
        lines = account.split('\n')

        # Extract account number and balance
        acc_number = lines[0].split(': ')[1]
        balance = float(lines[-1].split(': ')[1])

        # Check if the current account matches the provided account number
        if acc_number == account_number:
            # Update the balance based on withdrawal amount
            new_balance = max(0, balance - withdrawal_amount)

            # Replace the old balance with the new balance in the lines
            lines[-1] = f'Balance: {new_balance:.6f}'

            # Join the lines back into an account record
            updated_account = '\n'.join(lines)

            # Replace the old account record with the updated one
            data = data.replace(account, updated_account)

            # Write the updated data back to the file
            with open('Account.txt', 'w') as file:
                file.write(data)

            # Return the new balance
            return new_balance

    # If account number is not found, return None
    return None

def deposit(account_number, deposit_amount):
    # Read the contents of the text file
    with open('Account.txt', 'r') as file:
        data = file.read()

    # Split the data into individual account records
    accounts = data.split('\n\n')

    # Iterate through each account record
    for account in accounts:
        lines = account.split('\n')

        # Extract account number and balance
        acc_number = lines[0].split(': ')[1]
        balance = float(lines[-1].split(': ')[1])

        # Check if the current account matches the provided account number
        if acc_number == account_number:
            # Update the balance by adding the deposit amount
            new_balance = balance + deposit_amount

            # Replace the old balance with the new balance in the lines
            lines[-1] = f'Balance: {new_balance:.6f}'

            # Join the lines back into an account record
            updated_account = '\n'.join(lines)

            # Replace the old account record with the updated one
            data = data.replace(account, updated_account)

            # Write the updated data back to the file
            with open('Account.txt', 'w') as file:
                file.write(data)

            # Return the new balance
            return new_balance

    # If account number is not found, return None
    return None

def signUp():
    country_names = [country.name for country in pycountry.countries]
    layout_signup = [
        [sg.Text("First Name: "),sg.Input(key='-FIRSTNAME-')],
        [sg.Text("Last Name: "),sg.Input(key='-LASTNAME-')],
        [sg.Text("Middle Name: "),sg.Input(key='-MIDDLENAME-')],
        [sg.Text("Birthday: "),sg.InputText(key='-DATE-', enable_events=True, disabled=True)],
        [sg.CalendarButton('Choose Date', target='-DATE-', format='%Y-%m-%d', key='-CALENDAR-', pad=((8, 0), (10, 10)))],
        [sg.Text("Select Country: "),sg.Combo(country_names, key='-COUNTRY-', default_value='Philippines')],
        [sg.Text("Phone Number: "),sg.Input(key='-PNUMBER-', default_text='+63')],
        [sg.Text("Pin Number: "),sg.Input(key='-PIN-', password_char='*')],
        [sg.Text("Enter Initial Deposit(PHP): "), sg.Input(key='-BALANCE-')],
        [sg.Button("Submit", key='-SUBMIT-'), sg.Button("Cancel", key='-CANCEL-')]
    ]

    window_signup = sg.Window("Sign Up", layout_signup, resizable=True, element_justification='r')

    while True:
        event_signup, values_signup = window_signup.read()

        if event_signup == sg.WIN_CLOSED or event_signup == '-CANCEL-':
            break

        elif event_signup == '-SUBMIT-':
            
            
            #This will allocate and free random account number
            account_number = banking.generateRandomAccountNumber
            account_number.restype = ctypes.POINTER(ctypes.c_char_p)
            
            free_func = banking.freeMemory
            free_func.argtypes = [ctypes.POINTER(ctypes.c_char_p)]
            
            account_number_pointer = account_number()
            just_account_number = ctypes.c_char_p.from_buffer(account_number_pointer)
            account_number_string = just_account_number.value
            
            #This will store and capitalize(some) sign up values
            first_name = values_signup['-FIRSTNAME-'].capitalize()
            middle_name = values_signup['-MIDDLENAME-'].capitalize()
            last_name = values_signup['-LASTNAME-'].capitalize()
            balance = (values_signup['-BALANCE-'])
            birth = values_signup['-DATE-']
            
            #validate birthday input
            if birth == '':
                sg.popup_ok('Error: You seem to have forgot inputting your bday :D')
                continue
            
            #This will calculate age
            selected_date = values_signup['-DATE-']
            age = calculate_age(selected_date)
            
            
            
            if not is_valid_name(first_name):
                sg.popup_ok('Error: Invalid first name. First name must only contain letters.')
                continue
            if not is_valid_name(middle_name):
                sg.popup_ok('Error: Invalid middle name. Middle name must only contain letters.')
                continue
            if not is_valid_name(last_name):
                sg.popup_ok('Error: Invalid last name. Last name must only contain letters.')
                continue
            
            
            #This will Validate phone number and extract country code
            phone_number = values_signup['-PNUMBER-']
            country_code = validate_phone_number(phone_number)
            if country_code is None:
                sg.popup_ok('Error: Invalid phone number')
                continue
            
            #This will validate pin
            pin = values_signup['-PIN-']
            if not is_valid_pin(pin):
                sg.popup_ok('Error: Invalid Pin. Pin must be 0 - 9 and 6 digits.')
                continue
            
            #validates input
            a_string = "holds a string lol"
            a_float = 1.0
            an_int = int(100)
            
            if (type(balance) == type(a_float) or type(balance) == type(an_int)) and balance < 2000:
                sg.popup_ok("Minimum initial deposit must be PHP 2000 and above.")
                continue
            elif type(balance) == type(a_string):
                sg.popup_ok("Invalid deposit!")
                continue
            else:
                balance = float(balance) #explicilty converting it to float
                
            if age >= 18:
                layout_confirmsignup = [
                    [sg.Text("Confirm sign up details: ")],
                    [sg.Text(f"Name: {first_name} {middle_name} {last_name}")],
                    [sg.Text(f"Birthday: {selected_date}")],
                    [sg.Text(f"Age: {age}")],
                    [sg.Text(f"Country: {values_signup['-COUNTRY-']}")],
                    [sg.Text(f"Phone Number: {country_code} {values_signup['-PNUMBER-']}")],
                    [sg.Text(f"Account Number:  {account_number_string.decode()}")],
                    [sg.Text(f"Pin: {values_signup['-PIN-']}")],
                    [sg.Text(f"Balance: ₱{balance}")],
                    [sg.Button("Confirm", key="-CONFIRM-"), sg.Button("Cancel", key="-CANCEL-")]
                ]
                
                window_confirmsignup = sg.Window("Confirmation", layout_confirmsignup)
                
                while True:
                    event_confirmsignup, values_confirmsignup = window_confirmsignup.read()

                    if event_confirmsignup == sg.WIN_CLOSED or event_confirmsignup == "-CANCEL-":
                        break
                    elif event_confirmsignup == "-CONFIRM-":
                        try:
                            
                            age = str(age) #convert age to string
                            banking.signUp(
                                first_name.encode(), last_name.encode(),
                                middle_name.encode(), selected_date.encode(), age.encode(),
                                values_signup['-COUNTRY-'].encode(), values_signup['-PNUMBER-'].encode(),
                                account_number_string, values_signup['-PIN-'].encode(), ctypes.c_double(balance)
                            )
                            sg.popup_ok("Sign up Success!!")
                            window_signup.close()
                            break
                        except Exception as e:
                            sg.popup_ok(f"Error: {e}")
                            print(f"Error: {e}")# For debugging :)

                window_confirmsignup.close()
            else:
                sg.popup_ok('Error: Age must be 18 or above')
                
            free_func(account_number_pointer) #Free Memory Allocated        
    window_signup.close()

def login():
    layout_login = [
        [sg.Text("Account Number: ")],
        [sg.Input(key='-ACCOUNT_NUMBER-')],
        [sg.Text("Pin: ")],
        [sg.Input(key='-PIN_NUMBER-')],
        [sg.Button("Login", key='-LOGIN-'), sg.Button("Cancel", key='-CANCEL_LOGIN-')]
    ]

    windows_login = sg.Window("Login", layout_login, resizable=True)
    
    while True:
        login_event, login_values = windows_login.read()

        if login_event == sg.WIN_CLOSED or login_event == '-CANCEL_LOGIN-':            
            break
        
        elif login_event == '-LOGIN-':
            #Make Username and Pin container
            account_number = login_values['-ACCOUNT_NUMBER-']
            login_pin = login_values['-PIN_NUMBER-']
            #validate Username and Pin
            banking.login_f.restype = ctypes.POINTER(UserProfile)
            p = banking.login_f(account_number.encode(), login_pin.encode())
            
            
            if p is not None:
                
                windows_login.close()
                
                layout_main = [
                    [sg.VPush()],
                    [sg.Text(f"Name: {p.contents.firstName.decode()}"), sg.Text(f"{p.contents.middleName.decode()}"), sg.Text(f"{p.contents.lastName.decode()}")],
                    [sg.Text(f"Birthday: {p.contents.birthday.decode()}")],
                    [sg.Text(f"Country: {p.contents.country.decode()}")],
                    [sg.Text(f"Contact Info: {p.contents.phoneNumber.decode()}")],
                    [sg.Text(f"Current Balance: ₱{p.contents.balance}", key='-BALANCE-')],
                    [sg.Button("Withdraw", key='-WITHDRAW-'), sg.Button("Deposit", key='-DEPOSIT-'), sg.Button("Log Out", key='-LOGOUT-')],
                    [sg.VPush()]
                ]
                
                windows_main = sg.Window("User Profile", layout_main, resizable=True, size=(500, 500), element_justification='c')
                
                while True:
                    main_event, main_value = windows_main.read()
                    current_balance = float(p.contents.balance)
                    
                    if main_event == sg.WIN_CLOSED:
                        break
                    elif main_event == '-LOGOUT-':
                        windows_main.close()
                        break
                    elif main_event == '-WITHDRAW-':
                        layout_withdraw = [
                            [sg.Text("Enter withdraw amount: "), sg.Input(key='-WITHDRAW_AMOUNT-')],
                            [sg.Button("Confirm", key='-WITHDRAW_CONFIRM-'), sg.Button("Cancel", key='-WITHDRAW_CANCEL-')]
                        ]
                        
                        withdraw_window = sg.Window("Withdraw", layout_withdraw)
                       
                        while True:
                            withdraw_event, withdraw_values = withdraw_window.read()
                             
                            if withdraw_event == sg.WIN_CLOSED or withdraw_event == '-WITHDRAW_CANCEL-':
                                withdraw_window.close()
                                break
                            
                            elif withdraw_event == '-WITHDRAW_CONFIRM-':
                                if float(withdraw_values['-WITHDRAW_AMOUNT-']) > current_balance:
                                    sg.popup_ok("Withdraw Amount greater than current balance!")
                                    continue
                                else:
                                    try:
                                        new_balance = withdraw(login_values['-ACCOUNT_NUMBER-'], float(withdraw_values['-WITHDRAW_AMOUNT-']))
                                    except Exception as e:
                                        
                                        sg.popup_ok("Invalid Amount!")
                                        print(f"Error: {e}")#for debugging
                                        continue
                                        
                                    if new_balance is not None:
                                        sg.popup_ok(f'Withdrawal successful. New balance: ₱{new_balance:.6f}')
                                        open_pyqt5_print_window()
                                        
                                        #update new balance
                                        windows_main['-BALANCE-'].update(f"Current Balance: ₱{new_balance:.6f}")
                                        withdraw_window.close()
                                        break
                                    
                                    else:
                                        sg.popup_ok('Withdraw error!.')
                                        withdraw_window.close()
                                        break
                                
                    elif main_event == '-DEPOSIT-':
                        layout_deposit = [
                        [sg.Text("Enter withdraw deposit: "), sg.Input(key='-DEPOSIT_AMOUNT-')],
                        [sg.Button("Confirm", key='-DEPOSIT_CONFIRM-'), sg.Button("Cancel", key='-DEPOSIT_CANCEL-')]
                    ]
                    
                        deposit_window = sg.Window("Deposit", layout_deposit)
                        
                        while True:
                            deposit_event, deposit_values = deposit_window.read()
                            
                            if deposit_event == sg.WIN_CLOSED or deposit_event == '-DEPOSIT_CANCEL-':
                                break
                            elif deposit_event == '-DEPOSIT_CONFIRM-':
                                
                                try:
                                    new_balance = deposit(login_values['-ACCOUNT_NUMBER-'], float(deposit_values['-DEPOSIT_AMOUNT-']))
                                except Exception as e:
                                    sg.popup_ok("Invalid Amount!")
                                    print(f"Error: {e}")#for debugging
                                    continue
                                
                                if new_balance is not None:
                                    sg.popup_ok(f'Deposit successful. New balance: ₱{new_balance:.6f}')
                                    #update new balance
                                    windows_main['-BALANCE-'].update(f"Current Balance: ₱{new_balance:.6f}")
                                    deposit_window.close()
                                    break 
                                else:
                                    sg.popup_ok('deposit error!.')
                                    deposit_window.close()
                                    break
                                    deposit_window.close()        
                            
            elif p is None:
                sg.popup_ok("Invalid Credentials!");
                continue

    windows_login.close()
            
            
    

#main function
def main():
    sg.theme('LightPurple')
    layout = [
        [sg.VPush(), sg.Push(), sg.Button("Login", key='-LOGIN-', size=(8, 1)), sg.Button("Sign Up", key='-SIGNUP-', size=(8, 1)), sg.Push(), sg.VPush()],
        [sg.Push(), sg.Button("Exit", key='-EXIT-', size=(8, 1)), sg.Push()]
    ]

    window = sg.Window("My Bank", layout, resizable=True, size=(500, 500))
    
    while True:
        event, values = window.read()

        if event in (sg.WIN_CLOSED, '-EXIT-'):
            break
        elif event == '-SIGNUP-':
            signUp()
        elif event == '-LOGIN-':
            login()
                    
    window.close()

if __name__ == '__main__':
    main()
