package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"

	"github.com/summitto/mpc-server/swigmpc"
	"gopkg.in/yaml.v2"
)

type Config struct {
	Host           string   `yaml:"host"`
	Port           int      `yaml:"port"`
	Ssl            bool     `yaml:"ssl"`
	Cert           string   `yaml:"cert"`
	Key            string   `yaml:"key"`
	AllowedOrigins []string `yaml:"allowed_origins"`
}

// LoadConfig loads the configuration from a yaml file
func (c *Config) LoadConfig(filename string) error {
	// Open config file
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	// Init new YAML decode
	d := yaml.NewDecoder(file)

	// Start YAML decoding from file
	if err := d.Decode(&c); err != nil {
		return err
	}

	return nil
}

type MPCRequest struct {
	Party       uint32 `json:"party"`
	Port        uint16 `json:"port"`
	InputFolder string `json:"input_folder"`
}

type MPCResponse struct {
	Result []bool `json:"result"`
}

func handleMPC(rw http.ResponseWriter, r *http.Request) {
	defer r.Body.Close()

	dec := json.NewDecoder(r.Body)

	var req MPCRequest
	err := dec.Decode(&req)
	if err != nil {
		log.Println("Failed to decode HTTP request:", err)
		rw.WriteHeader(http.StatusBadRequest)
		rw.Write([]byte(err.Error()))
		return
	}

	if req.Party == 0 {
		rw.WriteHeader(http.StatusBadRequest)
		return
	}

	result := swigmpc.Perform_mpc(int(req.Party), int(req.Port), req.InputFolder)
	if result.Size() == 0 {
		log.Println("MPC returned an empty response, it likely didn't work because of you")
		rw.WriteHeader(http.StatusInternalServerError)
	}
	var response MPCResponse
	response.Result = make([]bool, result.Size())
	for i := 0; i < int(result.Size()); i++ {
		response.Result[i] = result.Get(i)
	}
	swigmpc.DeleteBoolVector(result)

	responseBytes, err := json.Marshal(response)
	if err != nil {
		log.Println("Failed to encode HTTP response:", err)
		rw.WriteHeader(http.StatusInternalServerError)
		rw.Write([]byte(err.Error()))
		return
	}

	rw.Header().Set("Content-Type", "application/json")
	log.Println(string(responseBytes))
	rw.Write(responseBytes)
}

func main() {
	defer func() {
		// an alternative to os.Exit, which respects defer calls
		err := recover()
		if err != nil {
			if code, ok := err.(int); ok {
				os.Exit(code)
			}

			panic(err) // not an exit code, bubble up panic
		}
	}()

	var config Config
	err := config.LoadConfig("config.yml")
	if err != nil {
		log.Println("Failed to open config:", err)
		panic(1)
	}

	log.Printf("Starting HTTP server on %s:%d\n", config.Host, config.Port)
	http.HandleFunc("/", handleMPC)
	err = http.ListenAndServe(fmt.Sprintf("%s:%d", config.Host, config.Port), http.DefaultServeMux)
	if err != nil {
		log.Println(err)
		panic(1)
	}
}
