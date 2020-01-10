import requests


def read_data_from_thingsboard():
    base = "https://demo.thingsboard.io/api/plugins/telemetry/DEVICE/"
    access_token = "bd2a9500-1d38-11ea-b102-b79db24c38c3"
    endpoint = "/values/attributes"
    url = base + access_token + endpoint

    headers = {
        'x-authorization': "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJmamdhcmNpYS5hbHZhcmV6QGhvdG1haWwuY29tIiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJ1c2VySWQiOiJkNjA2YWU1MC1lYWQwLTExZTktYmM3Mi1lOWQyMzA2NTUzOTYiLCJmaXJzdE5hbWUiOiJGcmFuY2lzY28gSmF2aWVyIiwibGFzdE5hbWUiOiJHYXJjaWEgQWx2YXJleiIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImQ1ODA4ZTYwLWVhZDAtMTFlOS1iYzcyLWU5ZDIzMDY1NTM5NiIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAiLCJpc3MiOiJ0aGluZ3Nib2FyZC5pbyIsImlhdCI6MTU3ODY1NDMyOCwiZXhwIjoxNTgwNDU0MzI4fQ.kTEpGzZbAMWk3YR3ItAEg8i6vIwajJi0tPCDmrH5SvMdO_1G3NUOhTiFpc_dsQtPhnR7OiIhdq2td-Q6-HuxPg",
        'content-type': "application/json"
        }

    r = requests.get(url=url, headers=headers)
    print(r.status_code)
    print(r.content)
    return r.content


if __name__ == "__main__":
    read_data_from_thingsboard()
