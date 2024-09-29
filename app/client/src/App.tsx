import { useState } from "react";
import {
  Container,
  Text,
  Stack,
  Title,
  Anchor,
  Group,
  TextInput,
  Button,
} from "@mantine/core";
import { useForm } from "@mantine/form";

export default function App() {
  const [isSubmitting, setIsSubmitting] = useState(false);
  const twisterBinary = "/twister";

  const userId = window.location.pathname.includes("/u/")
    ? window.location.pathname.split("/u/")[1]
    : undefined;
  const loggedIn = userId !== undefined;

  const form = useForm({
    initialValues: {
      flag: "",
    },
  });

  const handleSubmit = (values: typeof form.values) => {
    if (!/hack\{[A-Za-z0-9_]+\}/.test(values.flag)) {
      form.setErrors({ flag: "Flag doesn't match regex" });
      return;
    }
    setIsSubmitting(true);
    form.setFieldValue("flag", "");
    fetch("/api/submit", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ flag: values.flag, user: userId }),
    })
      .then((res) => res.json())
      .then((data) => {
        console.log(data);
      })
      .finally(() => {
        setIsSubmitting(false);
      });
  };

  return (
    <Container w="100%" maw="700px" my="100px">
      <Stack justify="center" align="center">
        <Title order={2}>Twister</Title>
        <Text mt={-12} c="dimmed">
          July 2024
        </Text>
        <Text mt={-20} c="dimmed">
          azliu0
        </Text>
        <Text fw={600} w="100%">
          Description
        </Text>
        <Text size="sm" ta="left" w="80%">
          <code>
            I need some help with my math homework. My mind is getting{" "}
            <i>twisted</i> looking at these problems.
          </code>
        </Text>
        <Text fw={600} w="100%">
          Binary
        </Text>
        <Text size="sm" ta="left" w="80%">
          <code>Download the binary </code>
          <code>
            <Anchor href={twisterBinary} download="twister">
              here
            </Anchor>
            .
          </code>
        </Text >
        <Text fw={600} w="100%">
          Remote
        </Text>
        <Text component="div" size="sm" ta="left" w="80%">
          <code>
            To get the flag, you'll need run your exploit against a remote
            server:
          </code>
          <Group
            justify="center"
            my="lg"
            p="10px"
            style={{
              border: "1px solid black",
            }}
          >
            <code>nc 3.81.92.37 4242</code>
          </Group>
        </Text>
        <Text fw={600} w="100%">
          Submit
        </Text>
        {
          loggedIn ? (
            <Text component="div" size="sm" ta="left" w="80%">
              <form onSubmit={form.onSubmit(handleSubmit)}>
                <Group w="100%" gap="0" align="top">
                  <TextInput
                    placeholder="hack{...}"
                    style={{
                      flexGrow: "1",
                    }}
                    {...form.getInputProps("flag")}
                  />
                  <Button
                    type="submit"
                    variant="outline"
                    color="black"
                    disabled={isSubmitting || !loggedIn}
                    style={{
                      flexGrow: "0",
                    }}
                  >
                    <Text fw={400}>
                      <code>Submit</code>
                    </Text>
                  </Button>
                </Group>
              </form>
            </Text>
          ) : (
            <Text size="sm" ta="left" w="80%">
              <code>Log in to submit</code>
            </Text>
          )
        }
        <Text fw={600} w="100%">
          Hints
        </Text>
        <Text component="div" size="sm" ta="left" w="80%">
          <ul style={{ listStyleType: "disc", paddingLeft: "14px" }}>
            <li>
              <code>The flag matches /hack&#123;[A-Za-z0-9_]+&#125;/</code>
            </li>
            <li>
              <code>
                The{" "}
                <Anchor
                  href="https://en.wikipedia.org/wiki/Mersenne_Twister"
                  target="_blank"
                >
                  Mersenne Twister
                </Anchor>{" "}
                is a pseudorandom number generator.
              </code>
            </li>
          </ul>
        </Text>
        <Text fw={600} w="100%">
          Other References
        </Text>
        <Text component="div" size="sm" ta="left" w="80%">
          <code>
            <Anchor
              href="https://ctf101.org/binary-exploitation/overview/"
              target="_blank"
            >
              Binary Exploitation Overview
            </Anchor>
            ,{" "}
            <Anchor
              href="https://ctf101.org/binary-exploitation/what-is-the-got/"
              target="_blank"
            >
              Global Offset Table
            </Anchor>
            ,{" "}
            <Anchor
              href="https://ctf101.org/binary-exploitation/what-is-a-format-string-vulnerability/"
              target="_blank"
            >
              Format String Vulnerability
            </Anchor>
            ,{" "}
            <Anchor
              href="https://github.com/Gallopsled/pwntools"
              target="_blank"
            >
              pwntools
            </Anchor>
          </code>
        </Text>
      </Stack>
    </Container >
  );
}
