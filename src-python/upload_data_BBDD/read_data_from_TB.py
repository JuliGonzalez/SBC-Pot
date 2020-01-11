# coding=utf-8

import requests
from configparser import ConfigParser
import json


def read_credentials(filename='../../config.ini', section='thingsboard'):
    parser = ConfigParser()
    parser.read(filename)

    thingsboard = {}
    if parser.has_section(section):
        items = parser.items(section)
        for item in items:
            thingsboard[item[0]] = item[1]
    else:
        raise Exception('{0} not found in the {1} file'.format(section, filename))

    return thingsboard


def read_data_from_thingsboard():
    thingsboard_credentials = read_credentials()
    base = "https://demo.thingsboard.io/api/plugins/telemetry/DEVICE/"
    access_token = thingsboard_credentials.get('access_token')
    endpoint = "/values/attributes"
    url = base + access_token + endpoint

    headers = {
        'x-authorization': "{}".format(thingsboard_credentials.get("jwt_token")),
        'content-type': "application/json",
    }

    r = requests.get(url=url, headers=headers)
    print(r.status_code)
    parsed = json.loads(r.content)
    print(parsed)
    print()
    return parsed


def return_data(json_data):
    data = []
    read_date = json[0].get("key")
    peso = json[0].get("key")
    humedad_suelo_INT = json[0].get("key")
    humedad_suelo_EXT = json[0].get("key")
    humedad_aire = json[0].get("key")
    co2 = json[0].get("key")
    luminosidad = json[0].get("key")
    temperatura = json[0].get("key")
    agua_detectada = json[0].get("key")
    data = [read_date, peso, humedad_suelo_INT, humedad_suelo_EXT, humedad_aire, co2, luminosidad, temperatura, agua_detectada]
    return data


if __name__ == "__main__":
    json = read_data_from_thingsboard()
    print(json[0].get("key"))

# todo
# forma correcta de hacer el post
# https://demo.thingsboard.io/api/v1/r7Sew3dL20m8iKCpzWIf/attributes
# {"attribute1":"value1", "attribute2":true, "attribute3":42.0, "attribute4":77}
# headers : Content-Type : application/json
