import csv
import sys
import os
import subprocess

# Definirea directoarelor de intrare și ieșire
INPUT_DIR = 'input'  # Asigură-te că acest director există
OUTPUT_DIR = 'output'  # Asigură-te că acest director există

total_score = 0  # Variabilă globală pentru a acumula scorurile

def clean_output_files():
    # Lista fișierelor de output care trebuie șterse
    output_files = [
        'profit_resursa.csv',
        'profit_total.csv',
        'clasament_rachete.csv',
        'misiuni.csv'  # Adăugat fișierul misiuni.csv
    ]
    
    for file in output_files:
        if os.path.exists(file):
            os.remove(file)  # Șterge fișierul dacă există

def read_asteroids_file(asteroids_file):
    asteroids_data = {}
    resources_from_asteroids = set()
    extracted_quantities = {}

    with open(asteroids_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            asteroid_id = row['ID Asteroid']
            resource = row['Resursă']
            quantity = float(row['Cantitate totală (kg)'])
            extraction_rate = float(row['Randament extracție (%)']) / 100
            
            asteroids_data[asteroid_id] = {
                'resource': resource,
                'total_quantity': quantity,
                'extraction_rate': extraction_rate,
                'distance': float(row['Distanță (km)'])
            }
            resources_from_asteroids.add(resource)

            if resource not in extracted_quantities:
                extracted_quantities[resource] = 0
            extracted_quantities[resource] += quantity * extraction_rate

    return asteroids_data, resources_from_asteroids, extracted_quantities

def read_market_file(market_file):
    market_data = {}
    with open(market_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            market_data[row['Resursă']] = {
                'min_price': float(row['Preț minim ($/kg)']),
                'max_price': float(row['Preț maxim ($/kg)'])
            }
    return market_data

def read_rockets_file(rockets_file):
    rockets_data = {}
    with open(rockets_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            rockets_data[row['ID Rachetă']] = {
                'capacity': float(row['Capacitate (kg)']),
                'fuel_consumption': float(row['Consum combustibil (kg/1000 km)']),
                'fuel_available': float(row['Combustibil disponibil (kg)'])
            }
    return rockets_data

def read_missions_file(missions_file):
    missions_data = []
    with open(missions_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            missions_data.append({
                'asteroid_id': row['ID Asteroid'],
                'rocket_id': row['ID Rachetă'],
                'resource': row['Resursă'],
                'extracted_quantity': float(row['Cantitate extrasă (kg)']),
                'profit_brut': float(row['Profit brut ($)'].replace(',', '')),
                'cost_combustibil': float(row['Cost combustibil ($)'].replace(',', '')),
                'profit_net': float(row['Profit net ($)'].replace(',', ''))
            })
    return missions_data

def validate_resources(asteroids_resources, profit_resources):
    for resource in asteroids_resources:
        if resource not in profit_resources:
            print(f"Error: Resource {resource} from asteroids file not found in profit file.")
            print("Output file: profit_resursa.csv")
            return False
    return True

def validate_profit_quantities(extracted_quantities, profit_file, market_data):
    with open(profit_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            resource = row['Resursă']
            reported_extracted_quantity = float(row['Cantitate extrasă (kg)'].replace(',', ''))
            calculated_extracted_quantity = extracted_quantities.get(resource, 0)

            if reported_extracted_quantity != calculated_extracted_quantity:
                print(f"Error: Extracted quantity for resource {resource} is incorrect. Expected: {calculated_extracted_quantity}, Found: {reported_extracted_quantity}")
                print("Output file: profit_resursa.csv")
                return False

            # Calcularea profitului brut
            if resource in market_data:
                max_price = market_data[resource]['max_price']
                min_price = market_data[resource]['min_price']
                
                # Calculăm profitul brut
                current_price = max_price  # Initialize current price to max price
                if reported_extracted_quantity <= 10000:
                    profit_brut_calculated = reported_extracted_quantity * current_price
                else:
                    profit_brut_calculated = 10000 * current_price
                    remaining_quantity = reported_extracted_quantity - 10000
                    current_price *= 0.9  # Update current price to 10% less
                    if current_price < min_price:  # Ensure it doesn't go below min price
                        current_price = min_price
                    while remaining_quantity > 0:
                        if remaining_quantity > 10000:
                            profit_brut_calculated += 10000 * current_price  # 10% discount
                            current_price *= 0.9  # Update current price to 10% less
                            if current_price < min_price:  # Ensure it doesn't go below min price
                                current_price = min_price
                            remaining_quantity -= 10000
                        else:
                            profit_brut_calculated += remaining_quantity * current_price  # 10% discount
                            remaining_quantity = 0

                # Asigură-te că profitul brut nu scade sub prețul minim
                if profit_brut_calculated < min_price * reported_extracted_quantity:
                    profit_brut_calculated = min_price * reported_extracted_quantity

                # Compară profitul brut calculat cu cel raportat
                reported_profit_brut = float(row['Profit brut ($)'].replace(',', ''))
                if abs(reported_profit_brut - profit_brut_calculated) > 0.001 * reported_profit_brut:  # 0.1% margin of error
                    print(f"Error: Profit brut for resource {resource} is incorrect. Expected: {profit_brut_calculated}, Found: {reported_profit_brut}")
                    print("Output file: profit_resursa.csv")
                    return False
    return True

def validate_test(rockets_file, asteroids_file, market_file, missions_file, profit_file, profit_total_file, clasament_file):
    asteroids_data, resources_from_asteroids, extracted_quantities = read_asteroids_file(asteroids_file)
    market_data = read_market_file(market_file)
    rockets_data = read_rockets_file(rockets_file)
    missions_data = read_missions_file(missions_file)

    profit_resources = set()
    with open(profit_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            profit_resources.add(row['Resursă'])

    if not validate_resources(resources_from_asteroids, profit_resources):
        return 0  # Returnează 0 dacă validarea resurselor eșuează

    if not validate_profit_quantities(extracted_quantities, profit_file, market_data):
        return 0  # Returnează 0 dacă validarea cantităților de profit eșuează

    validate_missions_data(missions_data, rockets_data, asteroids_data, market_data)

    if not verify_profit_total_file(profit_total_file, rockets_data, missions_file):
        return 0  # Returnează 0 dacă verificarea fișierului de profit total eșuează

    total_efficiency_score = verify_clasament_rachete_file(clasament_file, profit_total_file, missions_file)
    return total_efficiency_score  # Returnează scorul total de eficiență

def validate_missions_data(missions_data, rockets_data, asteroids_data, market_data):
    global test_passed
    for mission in missions_data:
        rocket_id = mission['rocket_id']
        asteroid_id = mission['asteroid_id']
        resources = mission['resource'].split('&')  # Împărțim resursele
        quantity = mission['extracted_quantity']
        profit_brut = mission['profit_brut']
        cost_combustibil = mission['cost_combustibil']
        profit_net = mission['profit_net']

        # Validare ID Rachetă
        if rocket_id not in rockets_data:
            print(f"Error: Rocket ID {rocket_id} not found.")
            print("Output file: misiuni.csv")
            test_passed = False

        # Validare ID Asteroid
        if asteroid_id not in asteroids_data:
            print(f"Error: Asteroid ID {asteroid_id} not found.")
            print("Output file: misiuni.csv")
            test_passed = False

        # Validare Resurse
        for resource in resources:
            if resource not in market_data:
                print(f"Error: Resource {resource} not found in market data.")
                print("Output file: misiuni.csv")
                test_passed = False

        # Validare Cantitate Extrasă
        if quantity < 0:
            print(f"Error: Extracted quantity for mission {rocket_id} to {asteroid_id} cannot be negative.")
            print("Output file: misiuni.csv")
            test_passed = False

        # Verificarea capacității rachetei
        if rocket_id in rockets_data:
            rocket_capacity = rockets_data[rocket_id]['capacity']
            if rocket_capacity <= 0:
                print(f"Error: Rocket {rocket_id} has non-positive capacity.")
                print("Output file: misiuni.csv")
                test_passed = False

        # Verificarea distanței
        if asteroid_id in asteroids_data:
            distance = asteroids_data[asteroid_id]['distance']
            if distance < 0:
                print(f"Error: Distance for asteroid {asteroid_id} cannot be negative.")
                print("Output file: misiuni.csv")
                test_passed = False

        # Verificarea cantității extrase, a capacității rachetei și a distanței
        if rocket_id in rockets_data:
            rocket_capacity = rockets_data[rocket_id]['capacity']
            fuel_available = rockets_data[rocket_id]['fuel_available']
            fuel_consumption = rockets_data[rocket_id]['fuel_consumption']

            if quantity > rocket_capacity:
                print(f"Error: Mission for {rocket_id} to {asteroid_id} with resources {', '.join(resources)} exceeds its capacity ({rocket_capacity}).")
                print("Output file: misiuni.csv")
                test_passed = False  # Setează test_passed pe False

            # Verificarea distanței
            if asteroid_id in asteroids_data:
                distance = asteroids_data[asteroid_id]['distance']
                max_range = (fuel_available / fuel_consumption) * 1000 * 2  # Distanța maximă dus-întors
                if distance > max_range:
                    print(f"Error: Rocket {rocket_id} cannot reach asteroid {asteroid_id} due to fuel constraints.")
                    print("Output file: misiuni.csv")
                    test_passed = False  # Setează test_passed pe False

        # Validare Profit Net
        calculated_profit_net = profit_brut - cost_combustibil
        if not (abs(calculated_profit_net - profit_net) <= 0.001 * abs(profit_net)):
            print(f"Error: Profit net for mission {rocket_id} to {asteroid_id} is incorrect. Expected: {calculated_profit_net}, Found: {profit_net}")
            print("Output file: misiuni.csv")
            test_passed = False

def verify_profit_total_file(profit_total_file, rockets_data, missions_file):
    seen_rockets = set()
    profit_data = {}

    # Citim datele din profit_total.csv
    with open(profit_total_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            rocket_id = row['ID Rachetă']
            if rocket_id in seen_rockets:
                print(f"Error: Rocket ID {rocket_id} appears multiple times in {profit_total_file}.")
                print("Output file: profit_total.csv")
                return False
            seen_rockets.add(rocket_id)

            # Stocăm profitul brut și costul combustibilului
            profit_brut = float(row['Profit brut ($)'].replace(',', ''))
            cost_total_combustibil = float(row['Cost total combustibil ($)'].replace(',', ''))
            profit_data[rocket_id] = (profit_brut, cost_total_combustibil)

    # Verificăm datele din fișierul de misiuni
    mission_data = {}
    with open(missions_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            rocket_id = row['ID Rachetă']
            profit_brut_misiune = float(row['Profit brut ($)'].replace(',', ''))
            cost_combustibil = float(row['Cost combustibil ($)'].replace(',', ''))
            profit_net_misiune = float(row['Profit net ($)'].replace(',', ''))

            if rocket_id not in mission_data:
                mission_data[rocket_id] = {
                    'total_profit_brut': 0,
                    'total_cost_combustibil': 0,
                    'total_profit_net': 0
                }

            mission_data[rocket_id]['total_profit_brut'] += profit_brut_misiune
            mission_data[rocket_id]['total_cost_combustibil'] += cost_combustibil
            mission_data[rocket_id]['total_profit_net'] += profit_net_misiune

    # Comparăm datele
    for rocket_id, totals in mission_data.items():
        if rocket_id in profit_data:
            expected_profit_brut, expected_cost_combustibil = profit_data[rocket_id]
            
            # Verificăm profitul brut cu marja de eroare
            if not (abs(totals['total_profit_brut'] - expected_profit_brut) <= 0.001 * expected_profit_brut):
                print(f"Error: Profit brut for rocket {rocket_id} is incorrect. Expected: {expected_profit_brut}, Found: {totals['total_profit_brut']}")
                print("Output file: profit_total.csv")
                return False
            
            # Verificăm costul combustibilului cu marja de eroare
            if not (abs(totals['total_cost_combustibil'] - expected_cost_combustibil) <= 0.001 * expected_cost_combustibil):
                print(f"Error: Cost total combustibil for rocket {rocket_id} is incorrect. Expected: {expected_cost_combustibil}, Found: {totals['total_cost_combustibil']}")
                print("Output file: profit_total.csv")
                return False
            
            # Verificăm profitul net cu marja de eroare
            calculated_profit_net = totals['total_profit_brut'] - totals['total_cost_combustibil']
            if not (abs(calculated_profit_net - profit_data[rocket_id][0] + profit_data[rocket_id][1]) <= 0.001 * abs(profit_data[rocket_id][0])):
                print(f"Error: Profit net for rocket {rocket_id} is incorrect. Expected: {calculated_profit_net}, Found: {totals['total_profit_net']}")
                print("Output file: profit_total.csv")
                return False
        else:
            print(f"Error: Rocket ID {rocket_id} from missions file not found in profit_total.csv.")
            print("Output file: profit_total.csv")
            return False

    return True

def verify_missions_file(missions_file):
    mission_data = {}
    
    with open(missions_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            rocket_id = row['ID Rachetă']
            profit_brut = float(row['Profit brut ($)'].replace(',', ''))
            cost_combustibil = float(row['Cost combustibil ($)'].replace(',', ''))
            profit_net = float(row['Profit net ($)'].replace(',', ''))

            if rocket_id not in mission_data:
                mission_data[rocket_id] = {
                    'total_profit_brut': 0,
                    'total_cost_combustibil': 0
                }

            mission_data[rocket_id]['total_profit_brut'] += profit_brut
            mission_data[rocket_id]['total_cost_combustibil'] += cost_combustibil

    # Verificăm profitul net
    for rocket_id, totals in mission_data.items():
        calculated_profit_net = totals['total_profit_brut'] - totals['total_cost_combustibil']
        if not (abs(calculated_profit_net - profit_net) <= 0.001 * abs(profit_net)):
            print(f"Error: Profit net for rocket {rocket_id} is incorrect. Expected: {calculated_profit_net}, Found: {profit_net}")
            print("Output file: misiuni.csv")
            return False

    return True

def verify_clasament_rachete_file(clasament_file, profit_total_file, missions_file):
    seen_rockets = set()
    profit_data = {}
    total_efficiency_score = 0  # Initialize total efficiency score

    # Citim datele din profit_total.csv
    with open(profit_total_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            rocket_id = row['ID Rachetă']
            profit_net = float(row['Profit net ($)'].replace(',', ''))
            profit_data[rocket_id] = profit_net

    # Citim datele din clasament_rachete.csv
    with open(clasament_file, 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            rocket_id = row['ID Rachetă']
            profit_net_clasament = float(row['Profit net ($)'].replace(',', ''))
            numar_misiuni = int(row['Număr misiuni'])
            scor_eficienta = float(row['Scor eficiență'].replace(',', ''))

            # Verificăm duplicate
            if rocket_id in seen_rockets:
                print(f"Error: Rocket ID {rocket_id} appears multiple times in {clasament_file}.")
                print("Output file: clasament_rachete.csv")
                return False
            seen_rockets.add(rocket_id)

            # Verificăm profitul net cu marja de eroare
            if rocket_id in profit_data:
                expected_profit_net = profit_data[rocket_id]
                if not (abs(profit_net_clasament - expected_profit_net) <= 0.001 * expected_profit_net):
                    print(f"Error: Profit net for rocket {rocket_id} is incorrect. Expected: {expected_profit_net}, Found: {profit_net_clasament}")
                    print("Output file: clasament_rachete.csv")
                    return False
            else:
                print(f"Error: Rocket ID {rocket_id} from clasament file not found in profit_total.csv.")
                print("Output file: clasament_rachete.csv")
                return False

            # Verificăm numărul de misiuni
            mission_count = 0
            with open(missions_file, 'r') as missions_f:
                missions_reader = csv.DictReader(missions_f, delimiter=';')
                for mission_row in missions_reader:
                    if mission_row['ID Rachetă'] == rocket_id:
                        mission_count += 1

            if mission_count != numar_misiuni:
                print(f"Error: Number of missions for rocket {rocket_id} is incorrect. Expected: {mission_count}, Found: {numar_misiuni}")
                print("Output file: clasament_rachete.csv")
                return False

            # Verificăm scorul de eficiență cu marja de eroare
            expected_scor_eficienta = expected_profit_net / mission_count if mission_count > 0 else 0
            if not (abs(scor_eficienta - expected_scor_eficienta) <= 0.001 * expected_scor_eficienta):
                print(f"Error: Efficiency score for rocket {rocket_id} is incorrect. Expected: {expected_scor_eficienta}, Found: {scor_eficienta}")
                print("Output file: clasament_rachete.csv")
                return False
            
            total_efficiency_score += scor_eficienta  # Add to total efficiency score

    return total_efficiency_score  # Return the total efficiency score

def run_executable(executable, test_number):
    # Rulăm executabilul pentru a genera fișierele de output
    try:
        result = subprocess.run([executable, 
                                 f"{INPUT_DIR}/test{test_number}/rachete.csv", 
                                 f"{INPUT_DIR}/test{test_number}/asteroizi.csv", 
                                 f"{INPUT_DIR}/test{test_number}/piata.csv"], 
                                check=True)  # Adăugat check=True pentru a verifica erorile
    except FileNotFoundError:
        print(f"Error: Executable '{executable}' not found. Please check the path and name.")
        sys.exit(1)
    if result.returncode != 0:
        print(f"Error running the executable for test {test_number}.")

def run_single_test(test_number, executable):
    global total_score  # Declarăm că vom folosi variabila globală
    clean_output_files()  # Curăță fișierele de output înainte de fiecare test
    print("=" * 30)  # Linie de separare
    print(f"        Test {test_number}        ")  # Mesaj de început pentru test
    print("=" * 30)  # Linie de separare
    print(f"Running test {test_number}...")
    
    # Rulăm executabilul pentru a genera fișierele de output
    run_executable(executable, test_number)

    rockets_file = f"{INPUT_DIR}/test{test_number}/rachete.csv"
    asteroids_file = f"{INPUT_DIR}/test{test_number}/asteroizi.csv"
    market_file = f"{INPUT_DIR}/test{test_number}/piata.csv"
    missions_file = 'misiuni.csv'  # Fișierul de output generat
    profit_file = 'profit_resursa.csv'  # Fișierele de output sunt în directorul curent
    profit_total_file = 'profit_total.csv'
    clasament_file = 'clasament_rachete.csv'

    score = validate_test(rockets_file, asteroids_file, market_file, missions_file, profit_file, profit_total_file, clasament_file)
    total_score += score  # Adăugăm scorul la total
    print(f"Test {test_number} score: {score:,}")  # Formatarea scorului cu virgule

    # Afișăm dacă testul a trecut sau a eșuat
    if score > 0:
        print("🎉 Test Passed! 🎉")
    else:
        print("❌ Test Failed! ❌")

def run_all_tests(executable):
    for test_number in range(7):  # Rulăm testele de la 0 la 6
        run_single_test(test_number, executable)
    
    print("=" * 30)  # Linie de separare
    print(f"    Total score from all tests: {total_score:,}    ")  # Afișăm suma totală a scorurilor cu virgule
    print("=" * 30)  # Linie de separare

def main():
    if len(sys.argv) != 2:  # Expecting one argument: the name of the executable
        print("Utilizare: python3 process_asteroids.py <nume_executabil>")
        sys.exit(1)

    executable = sys.argv[1]  # Numele executabilului este primit ca parametru
    run_all_tests(executable)

if __name__ == "__main__":
    main()