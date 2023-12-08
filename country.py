import PySimpleGUI as sg
import pycountry

# Get a list of country names from the pycountry library
country_names = [country.name for country in pycountry.countries]

sg.theme('LightPurple')

layout = [
    [sg.Text('Select a country:')],
    [sg.Combo(country_names, key='-COUNTRY-', default_value='United States')],
    [sg.Button('OK'), sg.Button('Exit')]
]

window = sg.Window('Country Dropdown Example', layout)

while True:
    event, values = window.read()

    if event in (sg.WIN_CLOSED, 'Exit'):
        break
    elif event == 'OK':
        selected_country = values['-COUNTRY-']
        sg.popup(f'You selected: {selected_country}')
print(type(selected_country))

window.close()


