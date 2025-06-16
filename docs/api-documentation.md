# AC Controller API Documentation

## Authentication

Control endpoints require API key authentication:

```
Authorization: ApiKey YOUR_API_KEY
```

API keys can be generated on the device's web dashboard.

**Note:** `/api/sensors` does not require authentication.

## Response Format

**IR Control endpoints** return:
```json
{
  "success": true|false,
  "error": "error message or empty string",
  "data": true|false
}
```

- **data**: `true` = command executed (automatic mode), `false` = command acknowledged but not executed (manual mode)

**Data endpoints** return:
```json
{
  "success": true|false,
  "error": "error message or empty string", 
  "data": { ... }
}
```

## Endpoints

### Turn Off AC

`POST /api/ir/off`

No request body required.

---

### Turn On AC

`POST /api/ir/on`

**Request:**
```json
{
  "mode": 1,
  "fanSpeed": 0,
  "temperature": 23,
  "swing": 1
}
```

**Parameters:**
- **mode** (integer, required):
  - `1`: Cool
  - `2`: Dry (ignores temperature)
  - `3`: Fan (ignores temperature)
  - `4`: Heat
- **fanSpeed** (integer, required):
  - `0`: Auto, `1`: High, `2`: Medium, `3`: Low, `4`: Quiet
- **temperature** (number, required):
  - Cool mode: 18-30°C
  - Heat mode: 16-30°C
  - Dry/Fan modes: ignored
- **swing** (integer, required):
  - `0`: Off, `1`: Vertical

---

### Toggle Powerful Mode

`POST /api/ir/toggle-powerful`

No request body required.

---

### Get Sensor Data

`GET /api/sensors`

**Response:**
```json
{
  "success": true,
  "error": "",
  "data": {
    "temperature": 22.5,
    "isAutomaticMode": true
  }
}
```

## HTTP Status Codes

- **200**: Success
- **400**: Invalid request data
- **401**: Authentication required