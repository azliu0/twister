import { Container, Text, Stack, Title, Anchor } from "@mantine/core";

export default function App() {
  return (
    <Container w="100%" maw="700px" mt="100px">
      <Stack justify="center" align="center">
        <Title order={2}>Twister</Title>
        <Anchor href="https://hacktimes.org" mt={-10} c="blue">
          HackTimes
        </Anchor>
        <Text mt={-20} c="dimmed">
          azliu0
        </Text>
        <Text size="sm" ta="center" w="80%">
          <code>Solve some mind-twisting math puzzles.</code>
        </Text>
        <Text fw={600} w="100%">
          Binary
        </Text>
        <Text size="sm" ta="left" w="80%">
          <code>Download the binary here:</code>
        </Text>
        <Text fw={600} w="100%">
          Remote
        </Text>
        <Text size="sm" ta="left" w="80%">
          <code>Download the binary here:</code>
        </Text>
        <Text fw={600} w="100%">
          Hints
        </Text>
        <Text size="sm" ta="left" w="80%">
          <code>Download the binary here:</code>
        </Text>
        <Text fw={600} w="100%">
          Submit
        </Text>
        <Text size="sm" ta="left" w="80%">
          <code>Download the binary here:</code>
        </Text>
      </Stack>
    </Container>
  );
}
