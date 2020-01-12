from mysql.connector import MySQLConnection, Error
from python_mysql_dbconfig import read_db_config


def insert_values(read_date, peso, humedad_suelo_INT, humedad_suelo_EXT, humedad_aire, co2, luminosidad, temperatura,
                  agua_detectada, rele):
    query = "INSERT INTO esp32.esp32_thingsboard(read_date, peso, humedad_suelo_INT, humedad_suelo_EXT, humedad_aire, co2, luminosidad, temperatura, agua_detectada, rele) " \
            "VALUES(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"
    args = (
        read_date, peso, humedad_suelo_INT, humedad_suelo_EXT, humedad_aire, co2, luminosidad, temperatura,
        agua_detectada, rele)

    try:
        db_config = read_db_config()
        conn = MySQLConnection(**db_config)

        cursor = conn.cursor()
        cursor.execute(query, args)

        if cursor.lastrowid:
            print('last insert id', cursor.lastrowid)
        else:
            print('last insert id not found')

        conn.commit()
    except Error as error:
        print("error")
        print(error)

    finally:
        cursor.close()
        conn.close()


def main():
    insert_values('2020-01-10-8:54', 29, 29, 29, 29, 29, 29, 29, 29, 0)


if __name__ == '__main__':
    main()
