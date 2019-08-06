package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"
	"unicode"
)

const (
	algo   = "../algo"
	lib    = "../lib"
	script = "../compile.sh"
	show   = "tspShow.cpp"
	omit   = `EDGE_WEIGHT_TYPE:`
	dim    = `DIMENSION:`
	result = "result"
)

func main() {
	root, err := os.Getwd()
	if err != nil {
		fmt.Println(err)
		return
	}
	os.Chdir(algo)

	sourceList, err := getSourceList(filepath.Join(root, algo))
	if err != nil {
		fmt.Println(err)
		return
	}

	// run script to create binaries
	for _, path := range sourceList {
		if err := compile(path); err != nil {
			fmt.Println(err)
			return
		}
		os.Chdir(filepath.Join(root, algo))
	}

	// save result
	for _, s := range sourceList {
		if _, err := os.Stat(filepath.Join(root, result, s[:len(s)-len("/enumTsp.cpp")])); os.IsNotExist(err) {
			os.Mkdir(filepath.Join(root, result, s[:len(s)-len("/enumTsp.cpp")]), 0777)
		}
	}
	// filter target test file
	os.Chdir(filepath.Join(root, algo))
	testList, err := filterTest(filepath.Join(root, lib))
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println("test filer passed")
	fmt.Println(sourceList)

	for _, source := range sourceList {
		fmt.Printf("%s ----------------------------", source)
		// exec file name is const value : enumTsp
		trim := len(source) - len(".cpp")
		exe := source[:trim]
		for _, test := range testList {
			fmt.Println(test)
			cmdstr := filepath.Join(algo, exe) + " -n -c " + " < " + filepath.Join(root, lib, test)
			out, err := exec.Command("sh", "-c", cmdstr).Output()
			if err != nil {
				fmt.Println("exec error:", err)
				continue
			}
			if err := outResult(out, filepath.Join(root, result, source[:len(source)-len("/enumTsp.cpp")], test[:len(test)-len(".tsp")])); err != nil {
				fmt.Println("write error:", err)
				continue
			}
			fmt.Println("done.")
		}
	}
}

// return algorithm file path
func getSourceList(dir string) ([]string, error) {
	var list []string
	files, err := ioutil.ReadDir(dir)
	if err != nil {
		return list, err
	}

	for _, file := range files {
		if file.IsDir() {
			list = append(list, filepath.Join(file.Name(), "enumTsp.cpp"))
		}
	}

	return list, nil
}

func compile(path string) error {
	os.Chdir(path)

	if err := exec.Command("bash", script, "../"+show, path).Run(); err != nil {
		return err
	}

	fmt.Println("Compile done.")
	return nil
}

func filterTest(dir string) ([]string, error) {
	var tests []string
	files, err := ioutil.ReadDir(dir)
	if err != nil {
		return tests, err
	}

	tsp := regexp.MustCompile(`.tsp`)

	for _, file := range files {
		if !tsp.MatchString(file.Name()) {
			continue
		}
		if ok, err := checkData(dir, file.Name()); !ok || err != nil {
			continue
		}
		tests = append(tests, file.Name())
	}

	return tests, nil
}

func checkData(dir, file string) (bool, error) {
	var ok bool = false
	f, err := os.Open(filepath.Join(dir, file))
	if err != nil {
		return ok, err
	}
	defer f.Close()

	or := regexp.MustCompile(omit)
	dr := regexp.MustCompile(dim)
	reader := bufio.NewReaderSize(f, 4096)
	for {
		line, _, err := reader.ReadLine()
		if err != nil {
			return false, err
		}
		l := string(line)
		l = strings.Map(func(r rune) rune {
			if unicode.IsSpace(r) {
				return -1
			}
			return r
		}, l)
		if or.MatchString(l) {
			l = l[len(omit):]
			if l == "EUC_2D" {
				ok = true
			}
			return ok, nil
		} else if dr.MatchString(l) {
			l = l[len(dim):]
			v, err := strconv.Atoi(l)
			if err != nil {
				return false, err
			}
			if v > 600 {
				return false, nil
			}
		}
	}
}

func outResult(out []byte, test string) error {
	file, err := os.Create(test)
	if err != nil {
		return err
	}
	defer file.Close()

	file.Write(out)
	return nil
}
